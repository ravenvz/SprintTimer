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
#ifndef BUTTONMENU_H
#define BUTTONMENU_H

#include "qt_gui/widgets/DataWidget.h"
#include <QPointer>
#include <QPushButton>
#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <core/IConfig.h>
#include <core/ICoreService.h>
#include <memory>

namespace Ui {
class LauncherMenu;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class LauncherMenu : public QWidget {

public:
    LauncherMenu(IConfig& applicationSettings,
                 ICoreService& coreService,
                 QWidget* parent);

    ~LauncherMenu() override;

public slots:
    void onSyncRequired();

private:
    std::unique_ptr<Ui::LauncherMenu> ui;
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

} // namespace sprint_timer::ui::qt_gui


#endif // SPRINT_TIMER_BUTTONMENU_H
