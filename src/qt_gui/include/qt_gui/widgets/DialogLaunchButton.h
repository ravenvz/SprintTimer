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
#ifndef DIALOGLAUNCHBUTTON_H_HAO5CGIQ
#define DIALOGLAUNCHBUTTON_H_HAO5CGIQ

#include "qt_gui/Displayable.h"
#include <QPushButton>

namespace sprint_timer::ui::qt_gui {

/* QPushButton extension that launches QDialog when clicked. */
class DialogLaunchButton : public QPushButton {
public:
    DialogLaunchButton(Displayable& dialog,
                       const QString& text,
                       QWidget* parent = nullptr);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: DIALOGLAUNCHBUTTON_H_HAO5CGIQ */
