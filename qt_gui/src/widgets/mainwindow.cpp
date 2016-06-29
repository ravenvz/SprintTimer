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
    pomodoroModel = new PomodoroModel(pomodoroService, this);
    ui->lvCompletedPomodoros->setModel(pomodoroModel);
    ui->lvCompletedPomodoros->setContextMenuPolicy(Qt::CustomContextMenu);
    taskModel = new TaskModel(pomodoroService, this);
    tagModel = new TagModel(pomodoroService, this);
    ui->lvTaskView->setModels(taskModel, tagModel);
    ui->lvTaskView->setContextMenuPolicy(Qt::CustomContextMenu);
    adjustUndoButtonState();
    setUiToIdleState();

    connect(ui->btnAddTodo, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::startTask);
    connect(
        ui->btnCancel, &QPushButton::clicked, this, &MainWindow::cancelTask);
    connect(ui->leDoneTask,
            &QLineEdit::returnPressed,
            this,
            &MainWindow::submitPomodoro);
    // Update selected task index and description of submission candidate
    connect(ui->lvTaskView, &QListView::clicked, [&](const QModelIndex& index) {
        selectedTaskIndex = index;
        setSubmissionCandidateDescription();
    });
    connect(ui->lvTaskView,
            &QListView::doubleClicked,
            this,
            &MainWindow::toggleTaskCompleted);
    connect(ui->btnZone,
            &QPushButton::clicked,
            this,
            &MainWindow::onInTheZoneToggled);
    connect(
        ui->leTask, &QLineEdit::returnPressed, this, &MainWindow::quickAddTask);
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
    connect(pomodoroModel,
            &PomodoroModel::modelReset,
            this,
            &MainWindow::updateDailyProgress);

    // Disables AddPomodoro button when there are no active tasks.
    connect(taskModel,
            &QAbstractListModel::modelReset,
            this,
            &MainWindow::adjustAddPomodoroButtonState);

    // Setup data synchronization signals
    connect(pomodoroModel,
            &AsyncListModel::updateFinished,
            taskModel,
            &AsyncListModel::synchronize);
    connect(taskModel,
            &AsyncListModel::updateFinished,
            pomodoroModel,
            &AsyncListModel::synchronize);
    connect(taskModel,
            &AsyncListModel::updateFinished,
            tagModel,
            &TagModel::synchronize);
    connect(taskModel,
            &AsyncListModel::updateFinished,
            this,
            &MainWindow::setSubmissionCandidateDescription);
    connect(tagModel,
            &AsyncListModel::updateFinished,
            pomodoroModel,
            &AsyncListModel::synchronize);
    connect(tagModel,
            &AsyncListModel::updateFinished,
            taskModel,
            &AsyncListModel::synchronize);
    connect(player.get(),
            static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(
                &QMediaPlayer::error),
            this,
            &MainWindow::onSoundError);

    // Invalidate selectedTaskIndex if the row it was pointing at was removed
    connect(taskModel,
            &QAbstractListModel::rowsRemoved,
            [&](const QModelIndex& parent, int first, int last) {
                if (selectedTaskIndex && (first <= selectedTaskIndex->row()
                                          && selectedTaskIndex->row() <= last))
                    selectedTaskIndex = optional<QModelIndex>();
            });
    connect(ui->pbUndo,
            &QPushButton::clicked,
            this,
            &MainWindow::onUndoButtonClicked);
    connect(pomodoroModel,
            &AsyncListModel::updateFinished,
            this,
            &MainWindow::adjustUndoButtonState);
    connect(taskModel,
            &AsyncListModel::updateFinished,
            this,
            &MainWindow::adjustUndoButtonState);
}

MainWindow::~MainWindow()
{
    delete historyView;
    delete statisticsView;
    delete goalsView;
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

void MainWindow::setTimerValue(Second timeLeft)
{
    QString timerValue = QString("%1:%2").arg(
        QString::number(timeLeft / secondsPerMinute),
        QString::number(timeLeft % secondsPerMinute).rightJustified(2, '0'));
    ui->labelTimer->setText(timerValue);
}

void MainWindow::adjustAddPomodoroButtonState()
{
    ui->btnAddPomodoroManually->setEnabled(taskModel->rowCount() != 0);
}

void MainWindow::playSound() const
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

void MainWindow::bringToForeground(QWidget* widgetPtr) const
{
    widgetPtr->raise();
    widgetPtr->activateWindow();
    widgetPtr->showNormal();
}

void MainWindow::onTimerTick(long timeLeft)
{
    emit timerUpdated(timeLeft / 1000);
}

/*********************** SLOTS *****************************/

void MainWindow::startTask()
{
    pomodoroTimer.run();
    setUiToRunningState();
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

void MainWindow::addTask()
{
    AddTaskDialog dialog{tagModel};
    if (dialog.exec()) {
        Task item = dialog.constructedTask();
        taskModel->insert(item);
    }
}

void MainWindow::quickAddTask()
{
    std::string encodedDescription = ui->leTask->text().toStdString();
    ui->leTask->clear();
    if (!encodedDescription.empty()) {
        Task item{std::move(encodedDescription)};
        taskModel->insert(item);
    }
}

void MainWindow::submitPomodoro()
{
    if (!selectedTaskIndex || ui->leDoneTask->text().isEmpty()) {
        qDebug() << "No associated Task can be found";
        return;
    }
    ui->leDoneTask->hide();
    completedTasksIntervals.push_back(pomodoroTimer.finish());
    for (const TimeSpan& timeSpan : completedTasksIntervals) {
        pomodoroModel->insert(
            timeSpan, taskModel->itemAt(selectedTaskIndex->row()).uuid());
    }

    completedTasksIntervals.clear();
    startTask();
}

void MainWindow::setSubmissionCandidateDescription()
{
    if (!selectedTaskIndex) {
        ui->leDoneTask->setText("");
        return;
    }
    auto task = taskModel->itemAt(selectedTaskIndex->row());
    QString description = QString("%1 %2")
                              .arg(QString::fromStdString(task.tagsAsString()))
                              .arg(QString::fromStdString(task.name()));
    ui->leDoneTask->setText(description);
}

void MainWindow::toggleTaskCompleted()
{
    taskModel->toggleCompleted(ui->lvTaskView->currentIndex());
}

void MainWindow::onInTheZoneToggled() { pomodoroTimer.toggleInTheZoneMode(); }

void MainWindow::launchSettingsDialog()
{
    SettingsDialog settingsDialog{applicationSettings};
    settingsDialog.fillSettingsData();
    if (settingsDialog.exec()) {
        qDebug() << "Applying changes";
    }
}

void MainWindow::launchHistoryView()
{
    if (!historyView) {
        historyView = new HistoryView(pomodoroService);
        connect(pomodoroModel,
                &AsyncListModel::updateFinished,
                historyView,
                &DataWidget::synchronize);
        connect(taskModel,
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
        connect(pomodoroModel,
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
        connect(pomodoroModel,
                &AsyncListModel::updateFinished,
                statisticsView,
                &DataWidget::synchronize);
        connect(taskModel,
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
    PomodoroManualAddDialog dialog{
        pomodoroModel, taskModel, applicationSettings.pomodoroDuration()};
    dialog.exec();
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

void MainWindow::updateDailyProgress()
{
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

void MainWindow::onSoundError(QMediaPlayer::Error error)
{
    QMessageBox::warning(
        this,
        "Sound playback error",
        QString("Error occured when trying to play sound file:\n %1\n\n%2")
            .arg(QString::fromStdString(applicationSettings.soundFilePath()))
            .arg(player->errorString()));
}

void MainWindow::onUndoButtonClicked()
{
    ConfirmationDialog dialog;
    QString description{"Revert following action:\n"};
    description.append(
        QString::fromStdString(pomodoroService.lastCommandDescription()));
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        pomodoroService.undoLast();
        pomodoroModel->synchronize();
        taskModel->synchronize();
        adjustUndoButtonState();
    }
}

void MainWindow::adjustUndoButtonState()
{
    ui->pbUndo->setVisible(pomodoroService.numRevertableCommands() == 0 ? false
                                                                        : true);
}
