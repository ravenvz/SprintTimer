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
#include "qt_gui/widgets/AutodisablingButton.h"

namespace {

void enableWidgetWhenModelHasData(QWidget& widget, QAbstractItemModel& model);

} // namespace

namespace sprint_timer::ui::qt_gui {

AutodisablingButton::AutodisablingButton(QAbstractItemModel& model_,
                                         const QString& text,
                                         QWidget* parent_)
    : QPushButton{text, parent_}
{
    enableWidgetWhenModelHasData(*this, model_);

    connect(&model_, &QAbstractItemModel::modelReset, [this, &model_]() {
        enableWidgetWhenModelHasData(*this, model_);
    });
}

} // namespace sprint_timer::ui::qt_gui

namespace {

void enableWidgetWhenModelHasData(QWidget& widget, QAbstractItemModel& model)
{
    widget.setEnabled(model.rowCount() > 0);
}

} // namespace
