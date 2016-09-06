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

#include "utils/WidgetUtils.h"
#include "dialogs/SettingsDialog.h"
#include "widgets/ButtonMenu.h"
#include "ui_button_menu.h"
#include "widgets/GoalProgressWindow.h"
#include "widgets/StatisticsWindow.h"
#include "widgets/HistoryWindow.h"

ButtonMenu::ButtonMenu(IConfig& applicationSettings,
                       ICoreService& coreService,
                       QWidget* parent)
    : ui{new Ui::ButtonMenu}
    , QWidget{parent}
    , settings{applicationSettings}
    , coreService{coreService}
{
    ui->setupUi(this);

    connect(ui->pbSettings,
            &QPushButton::clicked,
            this,
            &ButtonMenu::launchSettingsDialog);
    connect(ui->pbHistory,
            &QPushButton::clicked,
            this,
            &ButtonMenu::launchHistoryWindow);
    connect(ui->pbProgress,
            &QPushButton::clicked,
            this,
            &ButtonMenu::launchProgressWindow);
    connect(ui->pbStatistics,
            &QPushButton::clicked,
            this,
            &ButtonMenu::launchStatisticsWindow);
}

ButtonMenu::~ButtonMenu() {
    delete progressWindow;
    delete historyWindow;
    delete statisticsWindow;
    delete ui;
}

void ButtonMenu::launchHistoryWindow()
{
    if (!historyWindow) {
        historyWindow = new HistoryWindow(coreService);
        historyWindow->show();
    } else {
        WidgetUtils::bringToForeground(historyWindow);
    }
}

void ButtonMenu::launchSettingsDialog()
{
    SettingsDialog settingsDialog{settings};
    settingsDialog.exec();
}

void ButtonMenu::launchProgressWindow()
{
    if (!progressWindow) {
        progressWindow = new GoalProgressWindow(settings, coreService);
        progressWindow->show();
    } else {
        WidgetUtils::bringToForeground(progressWindow);
    }

}

void ButtonMenu::launchStatisticsWindow()
{
    if (!statisticsWindow) {
        statisticsWindow = new StatisticsWindow(settings, coreService);
        statisticsWindow->show();
    } else {
        WidgetUtils::bringToForeground(statisticsWindow);
    }
}

void ButtonMenu::onSprintDataChanged()
{
    if (progressWindow)
        progressWindow->synchronize();
    if (historyWindow)
        historyWindow->synchronize();
    if (statisticsWindow)
        statisticsWindow->synchronize();
}

void ButtonMenu::onTaskDataChanged()
{
    if (statisticsWindow)
        statisticsWindow->synchronize();
    if (historyWindow)
        historyWindow->synchronize();
}

void ButtonMenu::onTagDataChanged()
{
    if (statisticsWindow)
        statisticsWindow->synchronize();
    if (historyWindow)
        historyWindow->synchronize();
}

