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
#include <QDialog>
#include <core/IConfig.h>
#include <memory>

namespace Ui {
class LauncherMenu;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class LauncherMenu : public QWidget {

public:
    LauncherMenu(DataWidget& progressWindow,
                 DataWidget& statisticsWindow,
                 DataWidget& historyWindow,
                 QDialog& settingsDialog,
                 QWidget* parent = nullptr);

    ~LauncherMenu() override;

private:
    std::unique_ptr<Ui::LauncherMenu> ui;
};

} // namespace sprint_timer::ui::qt_gui


#endif // SPRINT_TIMER_BUTTONMENU_H
