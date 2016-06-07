#include "mainwindow.h"
#include "dialogs/AddTaskDialog.h"
#include "dialogs/confirmationdialog.h"
#include "dialogs/manualaddpomodorodialog.h"
#include "dialogs/settings_dialog.h"
#include "ui_mainwindow.h"
#include <QtWidgets/qmenu.h>
#include <thread>


MainWindow::MainWindow(IConfig& applicationSettings,
                       IPomodoroService& pomodoroService,
                       QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , applicationSettings(applicationSettings)
    , pomodoroService{pomodoroService}
    , pomodoroTimer{
          std::bind(&MainWindow::onTimerTick, this, std::placeholders::_1),
          1000,
          applicationSettings}
{
    ui->setupUi(this);
    player = std::make_unique<QMediaPlayer>();
    pomodoroModelNew = new PomodoroModel(pomodoroService, this);
    ui->lvCompletedPomodoros->setModel(pomodoroModelNew);
    ui->lvCompletedPomodoros->setContextMenuPolicy(Qt::CustomContextMenu);
    todoitemViewModel = new TodoItemModel(pomodoroService, this);
    ui->lvTodoItems->setModel(todoitemViewModel);

    todoitemViewDelegate = new TodoItemsViewDelegate(this);
    ui->lvTodoItems->setItemDelegate(todoitemViewDelegate);
    ui->lvTodoItems->setContextMenuPolicy(Qt::CustomContextMenu);

    tagModel = new TagModel(pomodoroService, this);

    setUiToIdleState();
    connectSlots();
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
    connect(pomodoroModelNew, &PomodoroModel::modelReset, this, &MainWindow::updateDailyProgress);
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
    progressBarMaxValue = pomodoroTimer.taskDuration() * secondsPerMinute;
    setTimerValue(progressBarMaxValue);
    ui->progressBar->setMaximum(progressBarMaxValue);
    ui->progressBar->setValue(0);
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
    if (pomodoroTimer.isBreak() || cancelDialog.exec()) {
        pomodoroTimer.cancel();
        setUiToIdleState();
    }
}

void MainWindow::addTodoItem()
{
    AddTodoItemDialog dialog{tagModel};
    if (dialog.exec()) {
        TodoItem item = dialog.constructedTask();
        todoitemViewModel->insert(item);
    }
}

void MainWindow::quickAddTodoItem()
{
    std::string encodedDescription = ui->leTodoItem->text().toStdString();
    ui->leTodoItem->clear();
    if (!encodedDescription.empty()) {
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
    pomodoroTimer.run();
    setUiToRunningState();
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
    // TODO move to config
    player->setMedia(QUrl::fromLocalFile(
        "/home/vizier/Projects/pomodoro_cpp/qt_gui/resources/ring.wav"));
    player->setVolume(applicationSettings.soundVolume());
    player->play();
}

void MainWindow::submitPomodoro()
{
    if (!selectedTask || ui->leDoneTask->text().isEmpty()) {
        qDebug() << "No associated Task can be found";
        return;
    }
    ui->leDoneTask->hide();
    completedTasksIntervals.push_back(pomodoroTimer.finish());
    for (const TimeSpan& timeSpan : completedTasksIntervals) {
        Pomodoro pomodoro{*selectedTask, timeSpan};
        pomodoroModelNew->insert(timeSpan, selectedTask->uuid());
    }

    completedTasksIntervals.clear();
    updateTaskList();
    updateOpenedWindows();
    startTask();
}

void MainWindow::updateDailyProgress()
{
    int dailyGoal = applicationSettings.dailyPomodorosGoal();
    if (dailyGoal == 0) {
        ui->labelDailyGoalProgress->hide();
        return;
    }
    int completedSoFar = pomodoroModelNew->rowCount();
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
        selectedTask = todoitemViewModel->itemAt(index.row());
        TodoItem item = todoitemViewModel->itemAt(index.row());
        QString description
            = QString("%1 %2")
                  .arg(QString::fromStdString(item.tagsAsString()))
                  .arg(QString::fromStdString(item.name()));
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
    TodoItem itemToEdit = todoitemViewModel->itemAt(index.row());
    AddTodoItemDialog dialog{tagModel};
    dialog.setWindowTitle("Edit TodoItem");
    dialog.fillItemData(itemToEdit);
    if (dialog.exec()) {
        TodoItem updatedItem = dialog.constructedTask();
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
        if (selectedTask
            && todoitemViewModel->itemAt(index.row()).uuid()
                == selectedTask->uuid()) {
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
        pomodoroModelNew->remove(index.row());
        // TODO replace with call to refresh
        todoitemViewModel->retrieveData();
    }
}

void MainWindow::toggleTodoItemCompleted()
{
    todoitemViewModel->toggleCompleted(ui->lvTodoItems->currentIndex());
    updateHistoryWindow();
}

void MainWindow::onInTheZoneToggled() { pomodoroTimer.toggleInTheZoneMode(); }

void MainWindow::launchHistoryView()
{
    if (!historyView) {
        historyView = new HistoryView(pomodoroService);
        historyView->show();
    }
    else {
        bringToForeground(historyView);
    }
}

void MainWindow::launchGoalsView()
{
    if (!goalsView) {
        goalsView = new GoalsView(applicationSettings, pomodoroService);
        goalsView->show();
    }
    else {
        bringToForeground(goalsView);
    }
}

void MainWindow::launchStatisticsView()
{
    if (!statisticsView) {
        statisticsView
            = new StatisticsWidget(applicationSettings, pomodoroService);
        statisticsView->show();
    }
    else {
        bringToForeground(statisticsView);
    }
}

void MainWindow::launchManualAddPomodoroDialog()
{
    PomodoroManualAddDialog dialog{pomodoroModelNew,
                                   todoitemViewModel,
                                   applicationSettings.pomodoroDuration()};
    if (dialog.exec()) {
        updateOpenedWindows();
        updateTaskList();
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
                SLOT(updateTaskList()));
        tagEditor->show();
    }
    else {
        bringToForeground(tagEditor);
    }
}

void MainWindow::updateTaskList() { todoitemViewModel->retrieveData(); }

void MainWindow::onTimerTick(long timeLeft)
{
    emit timerUpdated(timeLeft / 1000);
}

void MainWindow::onTimerUpdated(long timeLeft)
{
    if (timeLeft > 0) {
        int curVal{static_cast<int>(timeLeft)};
        ui->progressBar->setValue(progressBarMaxValue - curVal);
        setTimerValue(curVal);
        ui->progressBar->repaint();
        return;
    }
    playSound();

    if (ui->btnZone->isChecked()) {
        completedTasksIntervals.push_back(pomodoroTimer.finish());
        startTask();
    }
    else if (pomodoroTimer.isBreak()) {
        pomodoroTimer.finish();
        setUiToIdleState();
    }
    else {
        setUiToSubmissionState();
    }
}
