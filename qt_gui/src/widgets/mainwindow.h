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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core/IPomodoroService.h"
#include "core/IPomodoroStorageReader.h"
#include "core/IPomodoroYearRangeReader.h"
#include "core/IStorageImplementersFactory.h"
#include "goalsview.h"
#include "historyview.h"
#include "models/PomodoroModel.h"
#include "models/TagModel.h"
#include "models/TaskModel.h"
#include "statisticswidget.h"
#include "widgets/ITimerWidget.h"
#include <QMainWindow>
#include <QSettings>
#include <experimental/optional>
#include <functional>
#include <vector>

namespace Ui {
class MainWindow;
}


class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(IConfig& applicationSettings,
               IPomodoroService& pomodoroService,
               QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void timerUpdated(long timeLeft);

private:
    Ui::MainWindow* ui;
    IConfig& applicationSettings;
    IPomodoroService& pomodoroService;
    std::vector<TimeSpan> completedTasksIntervals;
    QPointer<PomodoroModel> pomodoroModel;
    QPointer<TagModel> tagModel;
    QPointer<TaskModel> taskModel;
    QPointer<DataWidget> goalsView;
    QPointer<StatisticsWidget> statisticsView;
    QPointer<HistoryView> historyView;
    std::experimental::optional<QModelIndex> selectedTaskIndex;
    ITimerWidget* timerWidget;

    void adjustAddPomodoroButtonState();
    void bringToForeground(QWidget* widgetPtr) const;

private slots:
    void addTask();
    void quickAddTask();
    void submitPomodoro(const std::vector<TimeSpan>& intervalBuffer);
    void setSubmissionCandidateDescription();
    void toggleTaskCompleted();
    void launchSettingsDialog();
    void launchHistoryView();
    void launchGoalsView();
    void launchStatisticsView();
    void launchManualAddPomodoroDialog();
    void updateDailyProgress();
    void onUndoButtonClicked();
    void adjustUndoButtonState();
    void showExp();
};

#endif // MAINWINDOW_H
