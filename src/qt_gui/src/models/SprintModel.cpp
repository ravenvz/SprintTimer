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
#include "qt_gui/models/SprintModel.h"
#include "qt_gui/metatypes/SprintDTOMetatype.h"
#include "qt_gui/models/CustomRoles.h"
#include <core/utils/StringUtils.h>

namespace {

QString sprintToString(const sprint_timer::ui::SprintDTO& sprint);

} // namespace

namespace sprint_timer::ui::qt_gui {

using dw::DateTime;
using dw::DateTimeRange;

SprintModel::SprintModel(QObject* parent_)
    : QAbstractListModel{parent_}
{
}

void SprintModel::displaySprints(const std::vector<SprintDTO>& sprints)
{
    beginResetModel();
    storage = sprints;
    endResetModel();
}

int SprintModel::rowCount(const QModelIndex& /*parent*/) const
{
    return static_cast<int>(storage.size());
}

QVariant SprintModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto& sprintRef = storage[static_cast<size_t>(index.row())];

    if (role == CustomRoles::ItemRole) {
        QVariant var;
        var.setValue(sprintRef);
        return var;
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    return sprintToString(sprintRef);
}

bool SprintModel::removeRows(int row, int count, const QModelIndex& index)
{
    if (count == 0)
        return false;

    if (row + count - 1 >= static_cast<int>(storage.size()))
        return false;

    // beginRemoveRows(index, row, row + count - 1);
    // storage.erase(storage.begin() + row, storage.begin() + row + count - 1);
    // endRemoveRows();

    if (auto p = presenter(); p) {
        p.value()->onSprintDelete(storage[row]);
        return true;
    }

    return false;
}

// bool SprintModel::setData(const QModelIndex& index,
//                             const QVariant& value,
//                             int role)
// {
//     if (!index.isValid())
//         return false;

// if (role == InsertRole) {
//     if (index.row() != static_cast<int>(storage.size()) - 1)
//         return false;
//     storage[index.row()] = value.value<Sprint>();
//     return true;
// }

//     return false;
// }

// bool SprintModel::insertRows(int row, int count, const QModelIndex& parent)
// {
//     // Only support inserting one item after the last row
//     if (row != static_cast<int>(storage.size()) || count != 1)
//         return false;
//
//     beginInsertRows(parent, rowCount(parent), rowCount(parent));
//     storage.emplace_back(SprintDTO{});
//     endInsertRows();
//     return true;
// }

} // namespace sprint_timer::ui::qt_gui

namespace {

QString sprintToString(const sprint_timer::ui::SprintDTO& sprint)
{
    const auto& timeSpan = sprint.timeRange;
    const auto& tags = sprint.tags;
    const auto prefixedTags =
        sprint_timer::utils::transformJoin(tags, " ", [](const auto& el) {
            std::string out;
            out += "#";
            out += el;
            return out;
        });
    return QString("%1 - %2 %3 %4")
        .arg(QString::fromStdString(dw::to_string(timeSpan.start(), "hh:mm")))
        .arg(QString::fromStdString(dw::to_string(timeSpan.finish(), "hh:mm")))
        .arg(QString::fromStdString(prefixedTags))
        .arg(QString::fromStdString(sprint.taskName));
}

} // namespace
