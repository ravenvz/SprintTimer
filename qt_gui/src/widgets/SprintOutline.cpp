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
#include "qt_gui/widgets/SprintOutline.h"
#include <QVBoxLayout>

namespace sprint_timer::ui::qt_gui {

SprintOutline::SprintOutline(QDialog& addSprintDialog,
                             QDialog& undoDialog,
                             std::unique_ptr<QPushButton> undoButton,
                             std::unique_ptr<QPushButton> addNewSprintButton,
                             std::unique_ptr<QListView> sprintView,
                             QWidget* parent)
    : QWidget{parent}
{
    auto layout = std::make_unique<QVBoxLayout>(this);

    connect(undoButton.get(), &QPushButton::clicked, [&undoDialog]() {
        undoDialog.open();
    });
    layout->addWidget(undoButton.release());

    layout->addWidget(sprintView.release());

    connect(addNewSprintButton.get(),
            &QPushButton::clicked,
            [&addSprintDialog]() { addSprintDialog.open(); });
    layout->addWidget(addNewSprintButton.release());

    setLayout(layout.release());
}

SprintOutline::~SprintOutline() = default;

} // namespace sprint_timer::ui::qt_gui
