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
#include "qt_gui/widgets/LauncherMenu.h"
#include "qt_gui/utils/WidgetUtils.h"
#include "ui_launcher_menu.h"

namespace sprint_timer::ui::qt_gui {

LauncherMenu::LauncherMenu(Displayable& progressWindow,
                           Displayable& statisticsWindow,
                           Displayable& historyWindow,
                           Displayable& settingsDialog,
                           QWidget* parent)
    : QWidget{parent}
    , ui{std::make_unique<Ui::LauncherMenu>()}
{
    ui->setupUi(this);

    connect(ui->pbHistory, &QPushButton::clicked, [&historyWindow]() {
        historyWindow.display();
    });
    connect(ui->pbProgress, &QPushButton::clicked, [&progressWindow]() {
        progressWindow.display();
    });
    connect(ui->pbStatistics, &QPushButton::clicked, [&statisticsWindow]() {
        statisticsWindow.display();
    });
    connect(ui->pbSettings, &QPushButton::clicked, [&settingsDialog]() {
        settingsDialog.display();
    });
}

LauncherMenu::~LauncherMenu() = default;

} // namespace sprint_timer::ui::qt_gui
