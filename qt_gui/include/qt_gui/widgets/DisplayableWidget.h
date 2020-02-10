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
#ifndef DISPLAYABLEWIDGET_H_EHFI9F4N
#define DISPLAYABLEWIDGET_H_EHFI9F4N

#include "qt_gui/Displayable.h"
#include <QWidget>

namespace sprint_timer::ui::qt_gui {

class DisplayableWidget : public QWidget, public Displayable {
public:
    explicit DisplayableWidget(QWidget* parent = nullptr);

    void display() override;

    bool isActive() const override;

    void dispose() override;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: DISPLAYABLEWIDGET_H_EHFI9F4N */
