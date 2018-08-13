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
#include <QGridLayout>
#include "qt_gui/widgets/MainWindow.h"
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

MainWindow::MainWindow(SprintOutline* sprintOutline,
                       TaskOutline* taskOutline,
                       TimerWidgetBase* timerWidget,
                       LauncherMenu* launcherMenu,
                       QWidget* parent)
    : QWidget{parent}
    , ui{std::make_unique<Ui::MainWindow>()}
    , sprintOutline{sprintOutline}
    , taskOutline{taskOutline}
    , launcherMenu{launcherMenu}
    , expansionState{shrinked.get()}
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(taskOutline, 0, 0, 3, 1);
    ui->gridLayout->addWidget(
        timerWidget, 1, 1, Qt::AlignHCenter | Qt::AlignTop);
    ui->gridLayout->addWidget(sprintOutline, 0, 2, 3, 1);
    ui->gridLayout->addWidget(launcherMenu, 4, 1, 1, 1, Qt::AlignHCenter);

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
