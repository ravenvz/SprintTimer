/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "mainwindow.h"
#include "dialogs/AddTaskDialog.h"
#include "dialogs/confirmationdialog.h"
#include "dialogs/manualaddpomodorodialog.h"
#include "dialogs/settings_dialog.h"
#include "ui_mainwindow.h"
#include "widgets/DefaultTimer.h"
#include "widgets/ExpandableWidget.h"
#include <QtWidgets/qmenu.h>


MainWindow::MainWindow(IConfig& applicationSettings,
                       IPomodoroService& pomodoroService,
                       QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
    , applicationSettings(applicationSettings)
    , pomodoroService{pomodoroService}
{
    ui->setupUi(this);
    timerWidget = new DefaultTimer{applicationSettings, this};
    ui->gridLayout->addWidget(timerWidget, 1, 1);
    pomodoroModel = new PomodoroModel(pomodoroService, this);
    ui->lvCompletedPomodoros->setModel(pomodoroModel);
    ui->lvCompletedPomodoros->setContextMenuPolicy(Qt::CustomContextMenu);
    taskModel = new TaskModel(pomodoroService, this);
    tagModel = new TagModel(pomodoroService, this);
    ui->lvTaskView->setModels(taskModel, tagModel);
    ui->lvTaskView->setContextMenuPolicy(Qt::CustomContextMenu);
    adjustUndoButtonState();

    connect(ui->pbAddTask, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(timerWidget,
            &TimerWidgetBase::submitRequested,
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
    connect(ui->pbSettings,
            &QPushButton::clicked,
            this,
            &MainWindow::launchSettingsDialog);
    connect(ui->pbHistory,
            &QPushButton::clicked,
            this,
            &MainWindow::launchHistoryView);
    connect(
        ui->pbGoals, &QPushButton::clicked, this, &MainWindow::launchGoalsView);
    connect(ui->pbStatistics,
            &QPushButton::clicked,
            this,
            &MainWindow::launchStatisticsView);
    connect(ui->pbAddPomodoroManually,
            &QPushButton::clicked,
            this,
            &MainWindow::launchManualAddPomodoroDialog);
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

void MainWindow::adjustAddPomodoroButtonState()
{
    ui->pbAddPomodoroManually->setEnabled(taskModel->rowCount() != 0);
}

void MainWindow::bringToForeground(QWidget* widgetPtr) const
{
    widgetPtr->raise();
    widgetPtr->activateWindow();
    widgetPtr->showNormal();
}

/*********************** SLOTS *****************************/

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
    std::string encodedDescription = ui->leQuickAddTask->text().toStdString();
    ui->leQuickAddTask->clear();
    if (!encodedDescription.empty()) {
        Task item{std::move(encodedDescription)};
        taskModel->insert(item);
    }
}

void MainWindow::submitPomodoro(const std::vector<TimeSpan>& intervalBuffer)
{
    // TODO see if it's enough
    // if (!selectedTaskIndex || ui->leDoneTask->text().isEmpty()) {
    if (!selectedTaskIndex) {
        qDebug() << "No associated Task can be found";
        return;
    }

    for (const TimeSpan& timeSpan : intervalBuffer) {
        pomodoroModel->insert(
            timeSpan, taskModel->itemAt(selectedTaskIndex->row()).uuid());
    }

    timerWidget->clearBuffer();
}

void MainWindow::setSubmissionCandidateDescription()
{
    if (!selectedTaskIndex) {
        timerWidget->setSubmissionCandidateDescription("");
        return;
    }
    auto task = taskModel->itemAt(selectedTaskIndex->row());
    QString description = QString("%1 %2")
                              .arg(QString::fromStdString(task.tagsAsString()))
                              .arg(QString::fromStdString(task.name()));
    timerWidget->setSubmissionCandidateDescription(description);
}

void MainWindow::toggleTaskCompleted()
{
    taskModel->toggleCompleted(ui->lvTaskView->currentIndex());
}

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

void MainWindow::updateDailyProgress()
{
    timerWidget->updateProgress(pomodoroModel->rowCount());
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

void MainWindow::showExp()
{
    ExpandableWidget* w = new ExpandableWidget();
    w->show();
}
