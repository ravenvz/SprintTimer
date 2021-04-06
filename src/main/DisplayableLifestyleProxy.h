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
#ifndef MANAGEDDISPLAYABLE_H_A4YKVFTJ
#define MANAGEDDISPLAYABLE_H_A4YKVFTJ

#include <QPointer>
#include <memory>
#include <qt_gui/StandaloneDisplayable.h>
#include <qt_gui/widgets/StandaloneDisplayableWidget.h>

namespace sprint_timer {

class DisplayableLifestyleProxy : public ui::qt_gui::StandaloneDisplayable {
public:
    void display() override
    {
        if (isActive()) {
            displayable->bringToTop();
            return;
        }
        if (!displayable) {
            displayable = create().release();
            displayable->setAttribute(Qt::WA_DeleteOnClose);
        }
        displayable->display();
    }

    ~DisplayableLifestyleProxy() override { delete displayable; }

private:
    QPointer<ui::qt_gui::StandaloneDisplayableWidget> displayable;

    bool isActive() const override
    {
        return displayable && displayable->isActive();
    }

    void bringToTop() override
    {
        if (displayable) {
            displayable->bringToTop();
        }
    }

    virtual std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget>
    create() = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: MANAGEDDISPLAYABLE_H_A4YKVFTJ */
