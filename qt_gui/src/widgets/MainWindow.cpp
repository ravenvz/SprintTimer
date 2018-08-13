/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "qt_gui/widgets/MainWindow.h"
#include "qt_gui/widgets/DefaultTimer.h"
#include "qt_gui/widgets/FancyTimer.h"
#include "qt_gui/widgets/LauncherMenu.h"
#include "qt_gui/widgets/SprintOutline.h"
#include "qt_gui/widgets/TaskOutline.h"
#include "qt_gui/widgets/TimerWidgetBase.h"
#include "ui_mainwindow.h"

namespace sprint_timer::ui::qt_gui {

namespace {
    auto expandedFully = std::make_unique<Expanded>();
    auto shrinked = std::make_unique<Shrinked>();
    auto expandedMenuOnly = std::make_unique<ExpandedMenuOnly>();
    auto expandedWithoutMenu = std::make_unique<ExpandedWithoutMenu>();
} // namespace

MainWindow::MainWindow(IConfig& applicationSettings,
                       ICoreService& coreService,
                       TaskModel& taskModel,
                       SprintModel& sprintModel,
                       TagModel& tagModel,
                       SprintOutline* sprintOutline,
                       TaskOutline* taskOutline,
                       TimerWidgetBase* timerWidget,
                       LauncherMenu* launcherMenu,
                       QWidget* parent)
    : QWidget(parent)
    , ui{std::make_unique<Ui::MainWindow>()}
    , taskModel{taskModel}
    , sprintModel{sprintModel}
    , sprintOutline{sprintOutline}
    , taskOutline{taskOutline}
    , timerWidget{timerWidget}
    , launcherMenu{launcherMenu}
    , expansionState{shrinked.get()}
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(taskOutline, 0, 0, 3, 1);
    ui->gridLayout->addWidget(
        timerWidget, 1, 1, Qt::AlignHCenter | Qt::AlignTop);
    ui->gridLayout->addWidget(sprintOutline, 0, 2, 3, 1);
    ui->gridLayout->addWidget(launcherMenu, 4, 1, 1, 1, Qt::AlignHCenter);

    connect(timerWidget,
            &TimerWidgetBase::submitRequested,
            this,
            &MainWindow::submitSprint);
    // Update selected task index and description of submission candidate
    connect(taskOutline, &TaskOutline::taskSelected, [&](const int row) {
        selectedTaskRow = row;
        timerWidget->setCandidateIndex(row);
    });
    connect(&sprintModel,
            &SprintModel::modelReset,
            this,
            &MainWindow::updateDailyProgress);
    connect(sprintOutline,
            &SprintOutline::actionUndone,
            launcherMenu,
            &LauncherMenu::onSyncRequired);

    // As models update data asynchroniously,
    // other models that depend on that data should
    // subscribe to updateFinished() signal
    connect(&sprintModel,
            &AsyncListModel::updateFinished,
            &taskModel,
            &AsyncListModel::synchronize);
    connect(&taskModel,
            &AsyncListModel::updateFinished,
            &sprintModel,
            &AsyncListModel::synchronize);
    connect(&taskModel,
            &AsyncListModel::updateFinished,
            &tagModel,
            &TagModel::synchronize);
    connect(&tagModel,
            &AsyncListModel::updateFinished,
            &sprintModel,
            &AsyncListModel::synchronize);
    connect(&tagModel,
            &AsyncListModel::updateFinished,
            &taskModel,
            &AsyncListModel::synchronize);

    connect(&taskModel,
            &QAbstractItemModel::rowsRemoved,
            this,
            &MainWindow::onTasksRemoved);
    connect(
        timerWidget,
        &TimerWidgetBase::submissionCandidateChanged,
        [&](int index) { selectedTaskRow = taskModel.index(index, 0).row(); });
    connect(&sprintModel,
            &AsyncListModel::updateFinished,
            launcherMenu,
            &LauncherMenu::onSyncRequired);
    connect(&taskModel,
            &AsyncListModel::updateFinished,
            launcherMenu,
            &LauncherMenu::onSyncRequired);

    connect(
        ui->pbToggleView, &QPushButton::clicked, this, &MainWindow::toggleView);
    connect(
        ui->pbToggleMenu, &QPushButton::clicked, this, &MainWindow::toggleMenu);

    setStateUi();
}

MainWindow::~MainWindow() = default;

void MainWindow::setStateUi()
{
    expansionState->setStateUi(*this);
    adjustSize();
}

void MainWindow::submitSprint(const std::vector<dw::TimeSpan>& intervalBuffer)
{
    if (!selectedTaskRow) {
        qDebug() << "No associated Task can be found";
        return;
    }

    for (const dw::TimeSpan& timeSpan : intervalBuffer) {
        sprintModel.insert(timeSpan, taskModel.itemAt(*selectedTaskRow).uuid());
    }
}

void MainWindow::updateDailyProgress()
{
    timerWidget->updateGoalProgress(sprintModel.rowCount(QModelIndex()));
}

QSize MainWindow::sizeHint() const { return expansionState->sizeHint(); }

void MainWindow::toggleView()
{
    expansionState->toggleView(*this);
    setStateUi();
}

void MainWindow::toggleMenu()
{
    expansionState->toggleMenu(*this);
    setStateUi();
}

void MainWindow::onTasksRemoved(const QModelIndex&, int first, int last)
{
    // If selectedTaskRow points to the row that has been removed,
    // we need to invalidate it.
    if (selectedTaskRow
        && (first <= selectedTaskRow && selectedTaskRow <= last)) {
        selectedTaskRow = std::optional<int>();
        timerWidget->setCandidateIndex(-1);
    }
}

ExpansionState::ExpansionState(int width, int height)
    : width{width}
    , height{height}
{
}

QSize ExpansionState::sizeHint() const { return QSize(width, height); }

Expanded::Expanded()
    : ExpansionState{812, 500}
{
}

void Expanded::setStateUi(MainWindow& widget)
{
    widget.taskOutline->setVisible(true);
    widget.sprintOutline->setVisible(true);
    widget.launcherMenu->setVisible(true);
    widget.ui->pbToggleView->setText("Collapse");
    widget.ui->pbToggleMenu->setText("Hide menu");
}

void Expanded::toggleView(MainWindow& widget)
{
    widget.expansionState = expandedMenuOnly.get();
}

void Expanded::toggleMenu(MainWindow& widget)
{
    widget.expansionState = expandedWithoutMenu.get();
}

Shrinked::Shrinked()
    : ExpansionState{300, 250}
{
}

void Shrinked::setStateUi(MainWindow& widget)
{
    widget.taskOutline->setVisible(false);
    widget.sprintOutline->setVisible(false);
    widget.launcherMenu->setVisible(false);
    widget.ui->pbToggleView->setText("Expand");
    widget.ui->pbToggleMenu->setText("Show menu");
}

void Shrinked::toggleView(MainWindow& widget)
{
    widget.expansionState = expandedWithoutMenu.get();
}

void Shrinked::toggleMenu(MainWindow& widget)
{
    widget.expansionState = expandedMenuOnly.get();
}

ExpandedMenuOnly::ExpandedMenuOnly()
    : ExpansionState{300, 250}
{
}

void ExpandedMenuOnly::setStateUi(MainWindow& widget)
{
    widget.taskOutline->setVisible(false);
    widget.sprintOutline->setVisible(false);
    widget.launcherMenu->setVisible(true);
    widget.ui->pbToggleView->setText("Expand");
    widget.ui->pbToggleMenu->setText("Hide menu");
}

void ExpandedMenuOnly::toggleView(MainWindow& widget)
{
    widget.expansionState = expandedFully.get();
}

void ExpandedMenuOnly::toggleMenu(MainWindow& widget)
{
    widget.expansionState = shrinked.get();
}

ExpandedWithoutMenu::ExpandedWithoutMenu()
    : ExpansionState{812, 450}
{
}

void ExpandedWithoutMenu::setStateUi(MainWindow& widget)
{
    widget.sprintOutline->setVisible(true);
    widget.taskOutline->setVisible(true);
    widget.launcherMenu->setVisible(false);
    widget.ui->pbToggleView->setText("Collapse");
    widget.ui->pbToggleMenu->setText("Show menu");
}

void ExpandedWithoutMenu::toggleView(MainWindow& widget)
{
    widget.expansionState = shrinked.get();
}

void ExpandedWithoutMenu::toggleMenu(MainWindow& widget)
{
    widget.expansionState = expandedFully.get();
}

} // namespace sprint_timer::ui::qt_gui
