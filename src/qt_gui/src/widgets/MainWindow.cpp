/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "ui_mainwindow.h"
#include <QGridLayout>

namespace {

const QString expandButtonText{"E&xpand"};
const QString collapseButtonText{"Co&llapse"};
const QString showMenuButtonText{"S&how menu"};
const QString hideMenuButtonText{"H&ide menu"};

} // namespace

// Single place to control MainWindow's widget sizes to simplify
// shrink-expand size configuration
namespace widget_size {

constexpr int baseSize{50};
constexpr int outlineWidth{baseSize * 7};
constexpr int outlineHeight{baseSize * 4};
const QSize outlineSize{outlineWidth, outlineHeight};
const QSize menuWidget{baseSize, baseSize * 2};
const QSize timerWidget{baseSize * 2, baseSize * 6};
constexpr int buttonsHeight{baseSize};
const QSize expandedMenuSize{timerWidget.width(),
                             timerWidget.height() + buttonsHeight +
                                 menuWidget.height()};
const QSize expandedOutlinesSize{2 * outlineWidth + timerWidget.width(),
                                 outlineHeight};
const QSize shrinkedSize{timerWidget.width(),
                         timerWidget.height() + buttonsHeight};
const QSize expandedSize{2 * outlineWidth + timerWidget.width(),
                         timerWidget.height() + menuWidget.height() +
                             buttonsHeight};

} // namespace widget_size

namespace sprint_timer::ui::qt_gui {

MainWindow::MainWindow(std::unique_ptr<QWidget> sprintOutline_,
                       std::unique_ptr<QWidget> taskOutline_,
                       std::unique_ptr<QWidget> todayProgressIndicator_,
                       std::unique_ptr<QWidget> timerWidget_,
                       std::unique_ptr<QWidget> launcherMenu_,
                       QWidget* parent)
    : QWidget{parent}
    , ui{std::make_unique<Ui::MainWindow>()}
    , sprintsWidget{sprintOutline_.release()}
    , tasksWidget{taskOutline_.release()}
    , menuWidget{launcherMenu_.release()}
    , size{widget_size::shrinkedSize}
    , fsm{TransitionTable{}, Setup{std::ref(*this)}}
{
    ui->setupUi(this);

    sprintsWidget->setMinimumSize(widget_size::outlineSize);
    sprintsWidget->setVisible(false);
    ui->gridLayout->addWidget(sprintsWidget, 0, 2, 3, 1);

    tasksWidget->setMinimumSize(widget_size::outlineSize);
    tasksWidget->setVisible(false);
    ui->gridLayout->addWidget(tasksWidget, 0, 0, 3, 1);

    ui->gridLayout->addWidget(todayProgressIndicator_.release(),
                              1,
                              1,
                              Qt::AlignHCenter | Qt::AlignBottom);
    timerWidget_->setMinimumSize(widget_size::timerWidget);
    ui->gridLayout->addWidget(
        timerWidget_.release(), 2, 1, Qt::AlignHCenter | Qt::AlignVCenter);

    menuWidget->setMinimumSize(widget_size::menuWidget);
    menuWidget->setVisible(false);
    ui->gridLayout->addWidget(menuWidget, 4, 1, 1, 1, Qt::AlignHCenter);

    connect(
        ui->pbToggleView, &QPushButton::clicked, this, &MainWindow::toggleView);
    connect(
        ui->pbToggleMenu, &QPushButton::clicked, this, &MainWindow::toggleMenu);

    adjustSize();
}

MainWindow::~MainWindow() = default;

auto MainWindow::sizeHint() const -> QSize { return size; }

void MainWindow::toggleView()
{
    fsm.process(ExpandButtonToggled{});
    adjustSize();
}

void MainWindow::toggleMenu()
{
    fsm.process(ExpandMenuButtonToggled{});
    adjustSize();
}

auto MainWindow::expandOutline() -> void
{
    sprintsWidget->setVisible(true);
    tasksWidget->setVisible(true);
    menuWidget->setVisible(false);
    ui->pbToggleView->setText(collapseButtonText);
    ui->pbToggleMenu->setText(showMenuButtonText);
    size = widget_size::expandedOutlinesSize;
}

auto MainWindow::shrink() -> void
{
    sprintsWidget->setVisible(false);
    tasksWidget->setVisible(false);
    menuWidget->setVisible(false);
    ui->pbToggleView->setText(expandButtonText);
    ui->pbToggleMenu->setText(showMenuButtonText);
    size = widget_size::shrinkedSize;
}

auto MainWindow::expand() -> void
{
    sprintsWidget->setVisible(true);
    tasksWidget->setVisible(true);
    menuWidget->setVisible(true);
    ui->pbToggleView->setText(collapseButtonText);
    ui->pbToggleMenu->setText(hideMenuButtonText);
    size = widget_size::expandedSize;
}

auto MainWindow::expandMenu() -> void
{
    sprintsWidget->setVisible(false);
    tasksWidget->setVisible(false);
    menuWidget->setVisible(true);
    ui->pbToggleView->setText(expandButtonText);
    ui->pbToggleMenu->setText(hideMenuButtonText);
    size = widget_size::expandedMenuSize;
}

} // namespace sprint_timer::ui::qt_gui
