#include "mainwindow.h"
#include "dialogs/AddTaskDialog.h"
#include "dialogs/confirmationdialog.h"
#include "dialogs/manualaddpomodorodialog.h"
#include "dialogs/settings_dialog.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtWidgets/qmenu.h>


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

    connect(
        ui->btnAddTodo, &QPushButton::clicked, this, &MainWindow::addTodoItem);
    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::startTask);
    connect(
        ui->btnCancel, &QPushButton::clicked, this, &MainWindow::cancelTask);
    connect(ui->leDoneTask,
            &QLineEdit::returnPressed,
            this,
            &MainWindow::submitPomodoro);
    connect(ui->lvTodoItems,
            &QListView::clicked,
            this,
            &MainWindow::changeSelectedTask);
    connect(ui->lvTodoItems,
            &QListView::customContextMenuRequested,
            this,
            &MainWindow::showTodoItemContextMenu);
    connect(ui->lvCompletedPomodoros,
            &QListView::customContextMenuRequested,
            this,
            &MainWindow::showPomodoroContextMenu);
    connect(ui->lvTodoItems,
            &QListView::doubleClicked,
            this,
            &MainWindow::toggleTodoItemCompleted);
    connect(ui->btnZone,
            &QPushButton::clicked,
            this,
            &MainWindow::onInTheZoneToggled);
    connect(ui->leTodoItem,
            &QLineEdit::returnPressed,
            this,
            &MainWindow::quickAddTodoItem);
    connect(ui->btnSettings,
            &QPushButton::clicked,
            this,
            &MainWindow::launchSettingsDialog);
    connect(ui->btnTodoHistory,
            &QPushButton::clicked,
            this,
            &MainWindow::launchHistoryView);
    connect(ui->btnGoals,
            &QPushButton::clicked,
            this,
            &MainWindow::launchGoalsView);
    connect(ui->btnStatistics,
            &QPushButton::clicked,
            this,
            &MainWindow::launchStatisticsView);
    connect(ui->btnAddPomodoroManually,
            &QPushButton::clicked,
            this,
            &MainWindow::launchManualAddPomodoroDialog);
    connect(this, &MainWindow::timerUpdated, this, &MainWindow::onTimerUpdated);
    connect(pomodoroModelNew,
            &PomodoroModel::modelReset,
            this,
            &MainWindow::updateDailyProgress);

    // Disables AddPomodoro button when there are no active tasks.
    connect(todoitemViewModel,
            &QAbstractListModel::modelReset,
            this,
            &MainWindow::adjustAddPomodoroButtonState);

    // Setup data synchronization signals
    connect(pomodoroModelNew,
            &AsyncListModel::updateFinished,
            todoitemViewModel,
            &AsyncListModel::synchronize);
    connect(todoitemViewModel,
            &AsyncListModel::updateFinished,
            pomodoroModelNew,
            &AsyncListModel::synchronize);
    connect(todoitemViewModel,
            &AsyncListModel::updateFinished,
            tagModel,
            &TagModel::synchronize);
    connect(tagModel,
            &AsyncListModel::updateFinished,
            pomodoroModelNew,
            &AsyncListModel::synchronize);
    connect(tagModel,
            &AsyncListModel::updateFinished,
            todoitemViewModel,
            &AsyncListModel::synchronize);
    connect(
        player.get(),
        static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(
            &QMediaPlayer::error),
        this, &MainWindow::onSoundError);
}

MainWindow::~MainWindow()
{
    delete historyView;
    delete statisticsView;
    delete goalsView;
    delete tagEditor;
    delete ui;
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
    ui->progressBar->setMaximum(progressBarMaxValue);
    setTimerValue(progressBarMaxValue);
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
    if (ui->btnZone->isChecked() || !applicationSettings.soundIsEnabled()) {
        return;
    }

    // TODO might not be the best way to handle this, as it requires
    // gstreamer-ugly-plugins on my system
    // TODO move to config
    player->setMedia(QUrl::fromLocalFile(
        QString::fromStdString(applicationSettings.soundFilePath())));
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
    }
}

void MainWindow::toggleTodoItemCompleted()
{
    todoitemViewModel->toggleCompleted(ui->lvTodoItems->currentIndex());
}

void MainWindow::onInTheZoneToggled() { pomodoroTimer.toggleInTheZoneMode(); }

void MainWindow::launchHistoryView()
{
    if (!historyView) {
        historyView = new HistoryView(pomodoroService);
        connect(pomodoroModelNew,
                &AsyncListModel::updateFinished,
                historyView,
                &DataWidget::synchronize);
        connect(todoitemViewModel,
                &AsyncListModel::updateFinished,
                historyView,
                &DataWidget::synchronize);
        connect(tagModel,
                &AsyncListModel::updateFinished,
                historyView,
                &DataWidget::synchronize);
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
        connect(pomodoroModelNew,
                &AsyncListModel::updateFinished,
                goalsView,
                &DataWidget::synchronize);
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
        connect(pomodoroModelNew,
                &AsyncListModel::updateFinished,
                statisticsView,
                &DataWidget::synchronize);
        connect(todoitemViewModel,
                &AsyncListModel::updateFinished,
                statisticsView,
                &DataWidget::synchronize);
        connect(tagModel,
                &AsyncListModel::updateFinished,
                statisticsView,
                &DataWidget::synchronize);
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
    dialog.exec();
}

void MainWindow::bringToForeground(QWidget* widgetPtr)
{
    widgetPtr->raise();
    widgetPtr->activateWindow();
    widgetPtr->showNormal();
}

void MainWindow::launchTagEditor()
{
    if (!tagEditor) {
        tagEditor = new TagEditorWidget{tagModel};
        tagEditor->show();
    }
    else {
        bringToForeground(tagEditor);
    }
}

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

void MainWindow::adjustAddPomodoroButtonState()
{
    ui->btnAddPomodoroManually->setEnabled(todoitemViewModel->rowCount() != 0);
}

void MainWindow::onSoundError(QMediaPlayer::Error error) {
    QMessageBox::warning(
        this,
        "Sound playback error",
        QString(
            "Error occured when trying to play sound file:\n %1\n\n%2")
            .arg(QString::fromStdString(
                applicationSettings.soundFilePath()))
            .arg(player->errorString()));
}
