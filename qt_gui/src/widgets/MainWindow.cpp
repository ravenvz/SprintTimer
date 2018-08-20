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
#include "qt_gui/widgets/LauncherMenu.h"
#include "qt_gui/widgets/SprintOutline.h"
#include "qt_gui/widgets/TaskOutline.h"
#include "qt_gui/widgets/TimerWidgetBase.h"
#include "ui_mainwindow.h"
#include <QGridLayout>

namespace {

const QString expand{"E&xpand"};
const QString collapse{"Co&llapse"};
const QString showMenu{"S&how menu"};
const QString hideMenu{"H&ide menu"};

} // namespace

// Single place to control MainWindow's widget sizes to simplify
// shrink-expand size configuration
namespace widget_size {

constexpr int outlineWidth{320};
constexpr int outlineHeight{200};
const QSize sprintOutline{outlineWidth, outlineHeight};
const QSize taskOutline{outlineWidth, outlineHeight};
const QSize launcherMenu{50, 100};
const QSize timer{100, 300};
constexpr int buttonsHeight{50};
const QSize expandedMenu{
    timer.width(), timer.height() + buttonsHeight + launcherMenu.height()};
const QSize expandedOutlines{2 * outlineWidth + timer.width(),
                             taskOutline.height()};
const QSize shrinked{timer.width(), timer.height() + buttonsHeight};
const QSize expanded{2 * outlineWidth + timer.width(),
                     timer.height() + launcherMenu.height() + buttonsHeight};

} // namespace widget_size

namespace sprint_timer::ui::qt_gui {

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
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(taskOutline, 0, 0, 3, 1);
    ui->gridLayout->addWidget(
        timerWidget, 1, 1, Qt::AlignHCenter | Qt::AlignTop);
    ui->gridLayout->addWidget(sprintOutline, 0, 2, 3, 1);
    ui->gridLayout->addWidget(launcherMenu, 4, 1, 1, 1, Qt::AlignHCenter);

    size = widget_size::shrinked;
    sprintOutline->setMinimumSize(widget_size::sprintOutline);
    taskOutline->setMinimumSize(widget_size::taskOutline);
    timerWidget->setMinimumSize(widget_size::timer);
    launcherMenu->setMinimumSize(widget_size::launcherMenu);

    connect(
        ui->pbToggleView, &QPushButton::clicked, this, &MainWindow::toggleView);
    connect(
        ui->pbToggleMenu, &QPushButton::clicked, this, &MainWindow::toggleMenu);

    sprintOutline->setVisible(false);
    taskOutline->setVisible(false);
    launcherMenu->setVisible(false);
    ui->pbToggleView->setText(expand);
    ui->pbToggleMenu->setText(showMenu);
    adjustSize();
}

MainWindow::~MainWindow() = default;

QSize MainWindow::sizeHint() const { return size; }

void MainWindow::toggleView()
{
    state_ = std::visit(ViewToggledEvent{*this}, state_);
    adjustSize();
}

void MainWindow::toggleMenu()
{
    state_ = std::visit(MenuToggledEvent{*this}, state_);
    adjustSize();
}

MainWindow::ExpandedOutlines::ExpandedOutlines(MainWindow& widget)
{
    widget.sprintOutline->setVisible(true);
    widget.taskOutline->setVisible(true);
    widget.launcherMenu->setVisible(false);
    widget.ui->pbToggleView->setText(collapse);
    widget.ui->pbToggleMenu->setText(showMenu);
    widget.size = widget_size::expandedOutlines;
}

MainWindow::Shrinked::Shrinked(MainWindow& widget)
{
    widget.sprintOutline->setVisible(false);
    widget.taskOutline->setVisible(false);
    widget.launcherMenu->setVisible(false);
    widget.ui->pbToggleView->setText(expand);
    widget.ui->pbToggleMenu->setText(showMenu);
    widget.size = widget_size::shrinked;
}

MainWindow::Expanded::Expanded(MainWindow& widget)
{
    widget.sprintOutline->setVisible(true);
    widget.taskOutline->setVisible(true);
    widget.launcherMenu->setVisible(true);
    widget.ui->pbToggleView->setText(collapse);
    widget.ui->pbToggleMenu->setText(hideMenu);
    widget.size = widget_size::expanded;
}

MainWindow::ExpandedMenu::ExpandedMenu(MainWindow& widget)
{
    widget.sprintOutline->setVisible(false);
    widget.taskOutline->setVisible(false);
    widget.launcherMenu->setVisible(true);
    widget.ui->pbToggleView->setText(expand);
    widget.ui->pbToggleMenu->setText(hideMenu);
    widget.size = widget_size::expandedMenu;
}

MainWindow::ViewToggledEvent::ViewToggledEvent(MainWindow& widget)
    : widget{widget}
{
}

MainWindow::State MainWindow::ViewToggledEvent::operator()(std::monostate)
{
    return ExpandedOutlines{widget};
}

MainWindow::State MainWindow::ViewToggledEvent::
operator()(const ExpandedOutlines&)
{
    return Shrinked{widget};
}

MainWindow::State MainWindow::ViewToggledEvent::operator()(const Shrinked&)
{
    return ExpandedOutlines{widget};
}

MainWindow::State MainWindow::ViewToggledEvent::operator()(const ExpandedMenu&)
{
    return Expanded{widget};
}

MainWindow::State MainWindow::ViewToggledEvent::operator()(const Expanded&)
{
    return ExpandedMenu{widget};
}

MainWindow::MenuToggledEvent::MenuToggledEvent(MainWindow& widget)
    : widget{widget}
{
}

MainWindow::State MainWindow::MenuToggledEvent::operator()(std::monostate)
{
    return ExpandedMenu{widget};
}

MainWindow::State MainWindow::MenuToggledEvent::
operator()(const ExpandedOutlines&)
{
    return Expanded{widget};
}

MainWindow::State MainWindow::MenuToggledEvent::operator()(const Shrinked&)
{
    return ExpandedMenu{widget};
}

MainWindow::State MainWindow::MenuToggledEvent::operator()(const ExpandedMenu&)
{
    return Shrinked{widget};
}

MainWindow::State MainWindow::MenuToggledEvent::operator()(const Expanded&)
{
    return ExpandedOutlines{widget};
}

} // namespace sprint_timer::ui::qt_gui
