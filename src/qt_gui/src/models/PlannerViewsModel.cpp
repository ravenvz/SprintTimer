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
#include "qt_gui/models/PlannerViewsModel.h"

namespace sprint_timer::ui::qt_gui {

PlannerViewsModel::PlannerViewsModel(QObject* parent_)
    : TreeModel{QStringList{"Name", "NumCompleted", "NumAll"}, parent_}
{
}

auto PlannerViewsModel::data(const QModelIndex& index,
                             int role) const -> QVariant
{
    if (not index.isValid()) {
        return {};
    }
    if (role == Qt::DisplayRole) {
        return TreeModel::data(index, role);
    }

    return {};
}

auto PlannerViewsModel::setData(const QModelIndex& index,
                                const QVariant& value,
                                int role) -> bool
{
    if (not index.isValid()) {
        return false;
    }

    if (role == Qt::EditRole) {
        return TreeModel::setData(index, value, role);
    }

    return false;
}

} // namespace sprint_timer::ui::qt_gui
