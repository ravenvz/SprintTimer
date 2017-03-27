/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core/ICoreService.h"
#include "core/ISprintStorageReader.h"
#include "core/IYearRangeReader.h"
#include "core/IStorageImplementersFactory.h"
#include "models/SprintModel.h"
#include "models/TagModel.h"
#include "models/TaskModel.h"
#include "widgets/TimerWidgetBase.h"
#include "widgets/TaskOutline.h"
#include "widgets/SprintOutline.h"
#include "widgets/LauncherMenu.h"
#include <QMainWindow>
#include <functional>
#include <memory>
#include <vector>
#include <QGridLayout>
#include "core/utils/OptionalPl.h"


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
    ~MainWindow() override;
    QSize sizeHint() const override;

private:
    Ui::MainWindow* ui;
    IConfig& applicationSettings;
    ICoreService& coreService;
    QPointer<SprintModel> sprintModel;
    QPointer<TagModel> tagModel;
    QPointer<TaskModel> taskModel;
    QPointer<LauncherMenu> launcherMenu;
    QPointer<TaskOutline> taskOutline;
    QPointer<SprintOutline> sprintOutline;
    optional<int> selectedTaskRow;
    TimerWidgetBase* timerWidget;
    ExpansionState* expansionState;

    void setStateUi();

private slots:
    void submitSprint(const std::vector<TimeSpan> &intervalBuffer);
    void updateDailyProgress();
    void toggleView();
    void toggleMenu();
    void onTasksRemoved(const QModelIndex&, int first, int last);
};


class ExpansionState {
public:
    ExpansionState(int width, int height);
    virtual ~ExpansionState() = default;
    QSize sizeHint() const;
    virtual void setStateUi(MainWindow& widget) = 0;
    virtual void toggleView(MainWindow& widget) = 0;
    virtual void toggleMenu(MainWindow& widget) = 0;

protected:
    const int width;
    const int height;
};


class Expanded final : public ExpansionState {
public:
    Expanded();
    void setStateUi(MainWindow& widget) override;
    void toggleView(MainWindow& widget) override;
    void toggleMenu(MainWindow& widget) override;
};


class Shrinked final : public ExpansionState {
public:
    Shrinked();
    void setStateUi(MainWindow& widget) override;
    void toggleView(MainWindow& widget) override;
    void toggleMenu(MainWindow& widget) override;
};


class ExpandedMenuOnly final : public ExpansionState {
public:
    ExpandedMenuOnly();
    void setStateUi(MainWindow& widget) override;
    void toggleView(MainWindow& widget) override;
    void toggleMenu(MainWindow& widget) override;
};


class ExpandedWithoutMenu final : public ExpansionState {
public:
    ExpandedWithoutMenu();
    void setStateUi(MainWindow& widget) override;
    void toggleView(MainWindow& widget) override;
    void toggleMenu(MainWindow& widget) override;
};

#endif // MAINWINDOW_H
