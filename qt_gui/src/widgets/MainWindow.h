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

#include "core/ICoreService.h"
#include "core/ISprintStorageReader.h"
#include "core/IYearRangeReader.h"
#include "core/IStorageImplementersFactory.h"
#include "HistoryWindow.h"
#include "models/SprintModel.h"
#include "models/TagModel.h"
#include "models/TaskModel.h"
#include "StatisticsWindow.h"
#include "widgets/TimerWidgetBase.h"
#include <QMainWindow>
#include <QSettings>
#include <experimental/optional>
#include <functional>
#include <memory>
#include <vector>

namespace Ui {
class MainWindow;
}


class ExpansionState;


class MainWindow : public QWidget {

    friend class ExpansionState;
    friend class Expanded;
    friend class Shrinked;
    friend class ExpandedMenuOnly;
    friend class ExpandedWithoutMenu;

public:
    MainWindow(IConfig& applicationSettings,
               ICoreService& coreService,
               QWidget* parent = nullptr);
    ~MainWindow();
    QSize sizeHint() const override;

private:
    Ui::MainWindow* ui;
    IConfig& applicationSettings;
    ICoreService& coreService;
    QPointer<SprintModel> sprintModel;
    QPointer<TagModel> tagModel;
    QPointer<TaskModel> taskModel;
    QPointer<DataWidget> goalsView;
    QPointer<DataWidget> statisticsView;
    QPointer<DataWidget> historyView;
    std::experimental::optional<QModelIndex> selectedTaskIndex;
    TimerWidgetBase* timerWidget;
    std::unique_ptr<ExpansionState> expandedFully;
    std::unique_ptr<ExpansionState> shrinked;
    std::unique_ptr<ExpansionState> expandedMenuOnly;
    std::unique_ptr<ExpansionState> expandedWithoutMenu;
    ExpansionState* expansionState;

    void adjustAddSprintButtonState();
    void bringToForeground(QWidget* widgetPtr) const;
    void setStateUi();

private slots:
    void addTask();
    void quickAddTask();
    void submitSprint(const std::vector<TimeSpan> &intervalBuffer);
    void toggleTaskCompleted();
    void launchSettingsDialog();
    void launchHistoryView();
    void launchGoalsView();
    void launchStatisticsView();
    void launchManualAddSprintDialog();
    void updateDailyProgress();
    void onUndoButtonClicked();
    void adjustUndoButtonState();
    void toggleView();
    void toggleMenu();
};


class ExpansionState {
public:
    ExpansionState(int width, int height, MainWindow& widget);
    virtual ~ExpansionState() = default;
    QSize sizeHint() const;
    virtual void setStateUi() = 0;
    virtual void toggleView() = 0;
    virtual void toggleMenu() = 0;

protected:
    const int width;
    const int height;
    MainWindow& widget;
};


class Expanded final : public ExpansionState {
public:
    Expanded(MainWindow& widget);
    void setStateUi();
    void toggleView();
    void toggleMenu();
};


class Shrinked final : public ExpansionState {
public:
    Shrinked(MainWindow& widget);
    void setStateUi();
    void toggleView();
    void toggleMenu();
};


class ExpandedMenuOnly final : public ExpansionState {
public:
    ExpandedMenuOnly(MainWindow& widget);
    void setStateUi();
    void toggleView();
    void toggleMenu();
};


class ExpandedWithoutMenu final : public ExpansionState {
public:
    ExpandedWithoutMenu(MainWindow& widget);
    void setStateUi();
    void toggleView();
    void toggleMenu();
};

#endif // MAINWINDOW_H
