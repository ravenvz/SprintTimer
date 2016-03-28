#include <thread>
#include "core/Timer.h"
#include <src/core/config.h>
#include <QtWidgets/qmenu.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogs/confirmationdialog.h"
#include "dialogs/addtodoitemdialog.h"
#include "dialogs/settings_dialog.h"
#include "dialogs/manualaddpomodorodialog.h"


MainWindow::MainWindow(TaskScheduler& scheduler,
                       IConfig& applicationSettings,
                       QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , taskScheduler(scheduler)
    , applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    player = std::make_unique<QMediaPlayer>();

    pomodoroModel = new PomodoroModel(this);
    pomodoroModel->setDateFilter(
        DateInterval{QDate::currentDate(), QDate::currentDate()});
    pomodoroModel->setSortByTime();
    pomodoroModel->select();
    ui->lvCompletedPomodoros->setModel(pomodoroModel);
    ui->lvCompletedPomodoros->setContextMenuPolicy(Qt::CustomContextMenu);

    todoitemViewModel = new TodoItemModel(this);
    todoitemViewModel->setNotCompletedFilter();
    todoitemViewModel->select();
    ui->lvTodoItems->setModel(todoitemViewModel);
    todoitemViewDelegate = new TodoItemsViewDelegate(this);
    ui->lvTodoItems->setItemDelegate(todoitemViewDelegate);
    ui->lvTodoItems->setContextMenuPolicy(Qt::CustomContextMenu);

    tagModel = new TagModel(this);

    setUiToIdleState();
    connectSlots();
    updatePomodoroView();
}

MainWindow::~MainWindow()
{
    delete todoitemViewModel;
    delete todoitemViewDelegate;
    delete historyView;
    delete statisticsView;
    delete goalsView;
    delete tagEditor;
    delete ui;
}

void MainWindow::connectSlots()
{
    connect(ui->btnAddTodo, SIGNAL(clicked(bool)), this, SLOT(addTodoItem()));
    connect(ui->btnStart, SIGNAL(clicked(bool)), this, SLOT(startTask()));
    connect(ui->btnCancel, SIGNAL(clicked(bool)), this, SLOT(cancelTask()));
    connect(
        ui->leDoneTask, SIGNAL(returnPressed()), this, SLOT(submitPomodoro()));
    connect(ui->lvTodoItems,
            SIGNAL(clicked(QModelIndex)),
            this,
            SLOT(changeSelectedTask(QModelIndex)));
    connect(ui->lvTodoItems,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(showTodoItemContextMenu(const QPoint&)));
    connect(ui->lvCompletedPomodoros,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(showPomodoroContextMenu(const QPoint&)));
    connect(ui->lvTodoItems,
            SIGNAL(doubleClicked(const QModelIndex&)),
            this,
            SLOT(toggleTodoItemCompleted()));
    connect(ui->btnZone, SIGNAL(clicked()), this, SLOT(onInTheZoneToggled()));
    connect(ui->leTodoItem,
            SIGNAL(returnPressed()),
            this,
            SLOT(quickAddTodoItem()));
    connect(ui->btnSettings,
            SIGNAL(clicked(bool)),
            this,
            SLOT(launchSettingsDialog()));
    connect(ui->btnTodoHistory,
            SIGNAL(clicked(bool)),
            this,
            SLOT(launchHistoryView()));
    connect(ui->btnGoals, SIGNAL(clicked(bool)), this, SLOT(launchGoalsView()));
    connect(ui->btnStatistics,
            SIGNAL(clicked(bool)),
            this,
            SLOT(launchStatisticsView()));
    connect(ui->btnAddPomodoroManually,
            SIGNAL(clicked(bool)),
            this,
            SLOT(launchManualAddPomodoroDialog()));
    connect(this, SIGNAL(timerUpdated(long)), this, SLOT(onTimerUpdated(long)));
}

void MainWindow::setUiToIdleState()
{
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    ui->leDoneTask->hide();
    ui->btnCancel->hide();
    ui->btnStart->show();
    ui->labelTimer->hide();
    progressBarMaxValue = 0;
}

void MainWindow::setUiToRunningState()
{
    setTimerValue(taskRunner.taskDuration() * secondsPerMinute);
    progressBarMaxValue = taskRunner.taskDuration() * secondsPerMinute;
    ui->progressBar->setMaximum(progressBarMaxValue);
    ui->btnStart->hide();
    ui->labelTimer->show();
    ui->progressBar->show();
    ui->btnCancel->show();
}

void MainWindow::setUiToSubmissionState()
{
    ui->labelTimer->hide();
    ui->progressBar->hide();
    ui->leDoneTask->show();
}

void MainWindow::cancelTask()
{
    ConfirmationDialog cancelDialog;
    QString description("This will destroy current pomodoro!");
    cancelDialog.setActionDescription(description);
    if (taskRunner.isBreak() || cancelDialog.exec()) {
        taskRunner.cancelTask();
        setUiToIdleState();
    }
}

void MainWindow::addTodoItem()
{
    AddTodoItemDialog dialog{tagModel};
    if (dialog.exec()) {
        TodoItem item = dialog.getNewTodoItem();
        todoitemViewModel->insert(item);
    }
}

void MainWindow::quickAddTodoItem()
{
    QString encodedDescription = ui->leTodoItem->text();
    ui->leTodoItem->clear();
    if (!encodedDescription.isEmpty()) {
        TodoItem item{std::move(encodedDescription)};
        todoitemViewModel->insert(item);
    }
}

void MainWindow::launchSettingsDialog()
{
    SettingsDialog settingsDialog{applicationSettings};
    settingsDialog.fillSettingsData();
    if (settingsDialog.exec()) {
        qDebug() << "Applying changes";
    }
}

void MainWindow::startTask()
{
    taskRunner.startTask();
    setUiToRunningState();
}

void MainWindow::updateTimerCounter()
{
    timeLeft--;
    if (timeLeft >= 0) {
        ui->progressBar->setValue(progressBarMaxValue - timeLeft);
        setTimerValue(timeLeft);
        ui->progressBar->repaint();
        return;
    }
    // timer->stop();
    playSound();

    if (ui->btnZone->isChecked()) {
        completedTasksIntervals.push_back(taskRunner.finishTask());
        startTask();
    }
    else if (taskScheduler.isBreak()) {
        taskRunner.finishTask();
        setUiToIdleState();
    }
    else {
        setUiToSubmissionState();
    }
}

void MainWindow::setTimerValue(Second timeLeft)
{
    QString timerValue = QString("%1:%2").arg(
        QString::number(timeLeft / secondsPerMinute),
        QString::number(timeLeft % secondsPerMinute).rightJustified(2, '0'));
    ui->labelTimer->setText(timerValue);
}

void MainWindow::playSound()
{
    if (ui->btnZone->isChecked() && !applicationSettings.soundIsEnabled()) {
        playSound();
    }

    // TODO might not be the best way to handle this, as it requires
    // gstreamer-ugly-plugins on my system
    player->setMedia(QUrl::fromLocalFile(
        "/home/vizier/Projects/pomodoro_cpp/resources/ring.wav"));
    player->setVolume(applicationSettings.soundVolume());
    player->play();
}

void MainWindow::submitPomodoro()
{
    if (!selectedTaskId || ui->leDoneTask->text().isEmpty()) {
        qDebug() << "No associated Task can be found";
        return;
    }
    ui->leDoneTask->hide();
    completedTasksIntervals.push_back(taskRunner.finishTask());
    for (const TimeInterval& interval : completedTasksIntervals) {
        pomodoroModel->insert(*selectedTaskId, interval);
    }

    completedTasksIntervals.clear();
    updateTodoItemModel();
    updatePomodoroView();
    updateOpenedWindows();
    startTask();
}

void MainWindow::updatePomodoroView()
{
    // QStringList lst = PomodoroDataSource::getPomodorosForToday();
    pomodoroModel->select();
    // pomodoroModel->setStringList(lst);
    int dailyGoal = applicationSettings.dailyPomodorosGoal();
    if (dailyGoal == 0) {
        ui->labelDailyGoalProgress->hide();
        return;
    }
    int completedSoFar = pomodoroModel->rowCount();
    ui->labelDailyGoalProgress->setText(
        QString("%1/%2").arg(completedSoFar).arg(dailyGoal));
    if (completedSoFar == dailyGoal) {
        ui->labelDailyGoalProgress->setStyleSheet("QLabel { color: green; }");
    }
    else if (completedSoFar > dailyGoal) {
        ui->labelDailyGoalProgress->setStyleSheet("QLabel { color: red; }");
    }
    else {
        ui->labelDailyGoalProgress->setStyleSheet("QLabel { color: black; }");
    }
}

void MainWindow::changeSelectedTask(QModelIndex index)
{
    // TODO consider having states like State::Submission instead
    if (ui->leDoneTask->isVisible()) {
        selectedTaskId = todoitemViewModel->itemIdAt(index.row());
        TodoItem item = todoitemViewModel->itemAt(index.row());
        QString description
            = QString("%1 %2").arg(item.tagsAsString()).arg(item.name());
        ui->leDoneTask->setText(description);
    }
}

void MainWindow::showTodoItemContextMenu(const QPoint& pos)
{
    QPoint globalPos = ui->lvTodoItems->mapToGlobal(pos);

    QMenu todoItemsMenu;
    // Note QMenu takes ownership of Action
    todoItemsMenu.addAction("Edit");
    todoItemsMenu.addAction("Delete");
    todoItemsMenu.addAction("Tag editor");

    QAction* selectedItem = todoItemsMenu.exec(globalPos);

    if (selectedItem && selectedItem->text() == "Edit")
        editTodoItem();
    if (selectedItem && selectedItem->text() == "Delete")
        removeTask();
    if (selectedItem && selectedItem->text() == "Tag editor")
        launchTagEditor();
}

void MainWindow::editTodoItem()
{
    QModelIndex index = ui->lvTodoItems->currentIndex();
    AddTodoItemDialog dialog{tagModel};
    TodoItem itemToEdit = todoitemViewModel->itemAt(index.row());
    dialog.setWindowTitle("Edit TodoItem");
    dialog.fillItemData(itemToEdit);
    if (dialog.exec()) {
        TodoItem updatedItem = dialog.getNewTodoItem();
        updatedItem.setSpentPomodoros(itemToEdit.spentPomodoros());
        updatedItem.setCompleted(itemToEdit.isCompleted());
        todoitemViewModel->replaceItemAt(index.row(), updatedItem);
    }
}

void MainWindow::removeTask()
{
    QModelIndex index = ui->lvTodoItems->currentIndex();
    ConfirmationDialog dialog;
    // TODO figure out a way to handle this situation more gracefully
    QString description;
    if (todoitemViewModel->itemAt(index.row()).spentPomodoros() > 0) {
        description
            = "WARNING! This todo item has pomodoros associated with it "
              "and they will be removed permanently along with this item.";
    }
    else {
        description = "This will delete todo item permanently!";
    }
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        // If removing currently selected task, clear the linedit
        if (selectedTaskId
            && todoitemViewModel->itemIdAt(index.row()) == *selectedTaskId) {
            ui->leDoneTask->clear();
        }
        todoitemViewModel->remove(index);
    }
}

void MainWindow::showPomodoroContextMenu(const QPoint& pos)
{
    QPoint globalPos = ui->lvCompletedPomodoros->mapToGlobal(pos);

    QMenu pomodoroMenu;
    pomodoroMenu.addAction("Delete");

    QAction* selectedItem = pomodoroMenu.exec(globalPos);

    if (selectedItem && selectedItem->text() == "Delete")
        removePomodoro();
}

void MainWindow::removePomodoro()
{
    QModelIndex index = ui->lvCompletedPomodoros->currentIndex();
    ConfirmationDialog dialog;
    QString description{"This will remove pomodoro permanently"};
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        // TODO handle sad path
        pomodoroModel->remove(index.row());
        // TODO replace with call to refresh
        todoitemViewModel->select();
    }
}

void MainWindow::toggleTodoItemCompleted()
{
    todoitemViewModel->toggleCompleted(ui->lvTodoItems->currentIndex());
    updateHistoryWindow();
}

void MainWindow::onInTheZoneToggled() { taskScheduler.toggleInTheZoneMode(); }

void MainWindow::launchHistoryView()
{
    if (!historyView) {
        historyView = new HistoryView();
        historyView->show();
    }
    else {
        bringToForeground(historyView);
    }
}

void MainWindow::launchGoalsView()
{
    if (!goalsView) {
        goalsView = new GoalsView(applicationSettings);
        goalsView->show();
    }
    else {
        bringToForeground(goalsView);
    }
}

void MainWindow::launchStatisticsView()
{
    if (!statisticsView) {
        statisticsView = new StatisticsWidget(applicationSettings);
        statisticsView->show();
    }
    else {
        bringToForeground(statisticsView);
    }
}

void MainWindow::launchManualAddPomodoroDialog()
{
    PomodoroManualAddDialog dialog{pomodoroModel,
                                   todoitemViewModel,
                                   applicationSettings.pomodoroDuration()};
    if (dialog.exec()) {
        updatePomodoroView();
        updateOpenedWindows();
        updateTodoItemModel();
    }
}

void MainWindow::bringToForeground(QWidget* widgetPtr)
{
    widgetPtr->raise();
    widgetPtr->activateWindow();
    widgetPtr->showNormal();
}

void MainWindow::updateOpenedWindows()
{
    updateStatisticsWindow();
    updateHistoryWindow();
    updateGoalWindow();
}

void MainWindow::updateStatisticsWindow()
{
    if (statisticsView)
        statisticsView->updateView();
}

void MainWindow::updateHistoryWindow()
{
    if (historyView)
        historyView->updateView();
}

void MainWindow::updateGoalWindow()
{
    if (goalsView)
        goalsView->updateView();
}

void MainWindow::launchTagEditor()
{
    if (!tagEditor) {
        tagEditor = new TagEditorWidget{tagModel};
        connect(tagEditor,
                SIGNAL(dataSetChanged()),
                this,
                SLOT(updateTodoItemModel()));
        tagEditor->show();
    }
    else {
        bringToForeground(tagEditor);
    }
}

void MainWindow::updateTodoItemModel() { todoitemViewModel->select(); }

void MainWindow::onTimerTick(long timeLeft)
{
    std::cout << "onTimerTick " << timeLeft << std::endl;
    emit timerUpdated(timeLeft / 1000);
}

void MainWindow::onTimerUpdated(long timeLeft)
{
    std::cout << "Time left " << timeLeft << " seconds" << std::endl;
    if (timeLeft > 0) {
        int curVal{static_cast<int>(timeLeft)};
        ui->progressBar->setValue(progressBarMaxValue - curVal);
        setTimerValue(curVal);
        ui->progressBar->repaint();
        return;
    }
    playSound();

    if (ui->btnZone->isChecked()) {
        completedTasksIntervals.push_back(taskRunner.finishTask());
        startTask();
    }
    else if (taskRunner.isBreak()) {
        taskRunner.finishTask();
        setUiToIdleState();
    }
    else {
        setUiToSubmissionState();
    }
}
