/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#ifndef LAUNCHERMENU_H_31QL4GCR
#define LAUNCHERMENU_H_31QL4GCR

#include "qt_gui/SprintTimerWidget.h"
#include <QDialog>
#include <core/IConfig.h>
#include <memory>

namespace Ui {
class LauncherMenu;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class LauncherMenu : public QWidget {

public:
    LauncherMenu(SprintTimerWidget& progressWindow,
                 QWidget& statisticsWindow,
                 QWidget& historyWindow,
                 QDialog& settingsDialog,
                 QWidget* parent = nullptr);

    ~LauncherMenu() override;

private:
    std::unique_ptr<Ui::LauncherMenu> ui;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: LAUNCHERMENU_H_31QL4GCR */
