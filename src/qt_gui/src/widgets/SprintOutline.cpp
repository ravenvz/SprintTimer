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
#include "qt_gui/widgets/SprintOutline.h"
#include <QPushButton>
#include <QVBoxLayout>

namespace sprint_timer::ui::qt_gui {

SprintOutline::SprintOutline(std::unique_ptr<QWidget> sprintView_,
                             std::unique_ptr<QWidget> undoWidget_,
                             Displayable& addSprintDialog_,
                             QWidget* parent_)
    : QWidget{parent_}
{
    auto layout = std::make_unique<QVBoxLayout>();
    auto addSprintButton = std::make_unique<QPushButton>("Add Sprint");

    connect(addSprintButton.get(),
            &QPushButton::clicked,
            [&addSprintDialog_]() { addSprintDialog_.display(); });

    layout->addWidget(undoWidget_.release());
    layout->addWidget(sprintView_.release());
    layout->addWidget(addSprintButton.release());

    setLayout(layout.release());
}

} // namespace sprint_timer::ui::qt_gui
