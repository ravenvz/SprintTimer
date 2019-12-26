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
#include "qt_gui/widgets/SubmissionBox.h"

namespace sprint_timer::ui::qt_gui {

SubmissionBox::SubmissionBox(IndexChangedReemitter& selectedTaskRowReemitter_,
                             QWidget* parent_)
    : QComboBox{parent_}
{
    connect(&selectedTaskRowReemitter_,
            &IndexChangedReemitter::currentRowChanged,
            [this](int row) {
                // Prevents signal emission when row hasn't changed
                if (currentIndex() != row)
                    setCurrentIndex(row);
            });
    connect(this,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this, &selectedTaskRowReemitter_](int index) {
                if (isVisible())
                    selectedTaskRowReemitter_.onRowChanged(index);
            });
}

void SubmissionBox::hideEvent(QHideEvent* event)
{
    persistentIndex = currentIndex();
    QComboBox::hideEvent(event);
}

void SubmissionBox::showEvent(QShowEvent* event)
{
    QComboBox::showEvent(event);
    setCurrentIndex(persistentIndex);
}

} // namespace sprint_timer::ui::qt_gui

