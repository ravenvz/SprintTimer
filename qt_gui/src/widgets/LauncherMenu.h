/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#ifndef BUTTONMENU_H
#define BUTTONMENU_H

#include <QWidget>
#include <QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <core/ICoreService.h>
#include "core/IConfig.h"
#include "widgets/DataWidget.h"
#include <QPointer>

namespace Ui {
class LauncherMenu;
} // namespace Ui

class LauncherMenu : public QWidget {

public:
    LauncherMenu(IConfig& applicationSettings,
               ICoreService& coreService,
               QWidget* parent);

    ~LauncherMenu();

public slots:
    void onSyncRequired();

private:
    Ui::LauncherMenu* ui;
    IConfig& settings;
    ICoreService& coreService;
    QPointer<DataWidget> progressWindow;
    QPointer<DataWidget> statisticsWindow;
    QPointer<DataWidget> historyWindow;

private slots:
    void launchSettingsDialog();
    void launchHistoryWindow();
    void launchProgressWindow();
    void launchStatisticsWindow();
};


#endif //SPRINT_TIMER_BUTTONMENU_H
