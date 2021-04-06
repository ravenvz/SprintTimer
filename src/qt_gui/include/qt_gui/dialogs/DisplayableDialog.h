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
#ifndef DISPLAYABLEDIALOG_H_ZLNTGNE6
#define DISPLAYABLEDIALOG_H_ZLNTGNE6

#include "qt_gui/Displayable.h"
#include <QDialog>

namespace sprint_timer::ui::qt_gui {

class DisplayableDialog : public QDialog, public Displayable {
public:
    explicit DisplayableDialog(QWidget* parent = nullptr)
        : QDialog{parent}
    {
    }

    void display() override { exec(); }

    bool isActive() const override { return isVisible(); }
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: DISPLAYABLEDIALOG_H_ZLNTGNE6 */
