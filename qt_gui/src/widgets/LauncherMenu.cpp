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
#include "qt_gui/widgets/LauncherMenu.h"
#include "qt_gui/dialogs/SettingsDialog.h"
#include "qt_gui/utils/WidgetUtils.h"
#include "qt_gui/widgets/GoalProgressWindow.h"
#include "qt_gui/widgets/HistoryWindow.h"
#include "qt_gui/widgets/StatisticsWindow.h"
#include "ui_launcher_menu.h"

namespace sprint_timer::ui::qt_gui {

LauncherMenu::LauncherMenu(IConfig& applicationSettings,
                           ICoreService& coreService,
                           QWidget* parent)
    : QWidget {parent}
    , ui {new Ui::LauncherMenu}
    , settings {applicationSettings}
    , coreService {coreService}
{
    ui->setupUi(this);

    connect(ui->pbSettings,
            &QPushButton::clicked,
            this,
            &LauncherMenu::launchSettingsDialog);
    connect(ui->pbHistory,
            &QPushButton::clicked,
            this,
            &LauncherMenu::launchHistoryWindow);
    connect(ui->pbProgress,
            &QPushButton::clicked,
            this,
            &LauncherMenu::launchProgressWindow);
    connect(ui->pbStatistics,
            &QPushButton::clicked,
            this,
            &LauncherMenu::launchStatisticsWindow);
}

LauncherMenu::~LauncherMenu()
{
    delete progressWindow;
    delete historyWindow;
    delete statisticsWindow;
    delete ui;
}

void LauncherMenu::launchHistoryWindow()
{
    if (!historyWindow) {
        historyWindow = new HistoryWindow(coreService);
        historyWindow->show();
    }
    else {
        WidgetUtils::bringToForeground(historyWindow);
    }
}

void LauncherMenu::launchSettingsDialog()
{
    SettingsDialog settingsDialog {settings};
    settingsDialog.exec();
}

void LauncherMenu::launchProgressWindow()
{
    if (!progressWindow) {
        progressWindow = new GoalProgressWindow(settings, coreService);
        progressWindow->show();
    }
    else {
        WidgetUtils::bringToForeground(progressWindow);
    }
}

void LauncherMenu::launchStatisticsWindow()
{
    if (!statisticsWindow) {
        statisticsWindow = new StatisticsWindow(settings, coreService);
        statisticsWindow->show();
    }
    else {
        WidgetUtils::bringToForeground(statisticsWindow);
    }
}

void LauncherMenu::onSyncRequired()
{
    if (progressWindow)
        progressWindow->synchronize();
    if (historyWindow)
        historyWindow->synchronize();
    if (statisticsWindow)
        statisticsWindow->synchronize();
}

} // namespace sprint_timer::ui::qt_gui
