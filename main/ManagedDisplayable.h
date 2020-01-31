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
#ifndef MANAGEDDISPLAYABLE_H_A4YKVFTJ
#define MANAGEDDISPLAYABLE_H_A4YKVFTJ

#include <memory>
#include <qt_gui/Displayable.h>

namespace sprint_timer {

class ManagedDisplayable : public ui::qt_gui::Displayable {
public:
    void display() override
    {
        if (displayable && displayable->isActive()) {
            displayable->bringToTop();
            return;
        }
        displayable = create();
        displayable->display();
    }

    void close() override { displayable.reset(nullptr); }

private:
    std::unique_ptr<Displayable> displayable;

    virtual std::unique_ptr<Displayable> create() = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: MANAGEDDISPLAYABLE_H_A4YKVFTJ */
