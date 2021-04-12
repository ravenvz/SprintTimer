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
#include <QPointer>
#include <memory>
#include "qt_gui/dialogs/DisplayableDialog.h"

#ifndef DISPLAYABLEDIALOGLIFESTYLEPROXY_H_IQLGLKYT
#define DISPLAYABLEDIALOGLIFESTYLEPROXY_H_IQLGLKYT

namespace sprint_timer::compose {

class DisplaybleDialogLifestyleProxy : public ui::qt_gui::Displayable {
public:
    void display() override
    {
        displayable = create().release();
        displayable->display();
    }

    bool isActive() const override { return displayable != nullptr; }

    ~DisplaybleDialogLifestyleProxy() override { delete displayable; }

private:
    QPointer<ui::qt_gui::DisplayableDialog> displayable;
    virtual std::unique_ptr<ui::qt_gui::DisplayableDialog> create() = 0;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: DISPLAYABLEDIALOGLIFESTYLEPROXY_H_IQLGLKYT */
