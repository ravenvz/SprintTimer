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
#include "qt_gui/models/PomodoroModel.h"
#include "qt_gui/metatypes/SprintMetaType.h"

namespace sprint_timer::ui::qt_gui {

using dw::DateTime;
using dw::DateTimeRange;
using entities::Sprint;
using entities::Tag;

PomodoroModel::PomodoroModel(QObject* parent_)
    : QAbstractListModel{parent_}
{
}

int PomodoroModel::rowCount(const QModelIndex& /*parent*/) const
{
    return static_cast<int>(storage.size());
}

QVariant PomodoroModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Sprint& sprintRef = storage[static_cast<size_t>(index.row())];

    if (role == ItemRole) {
        QVariant var;
        var.setValue(sprintRef);
        return var;
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    return QString("%1 - %2 %3 %4")
        .arg(dw::to_string(sprintRef.startTime(), "hh:mm").c_str())
        .arg(dw::to_string(sprintRef.finishTime(), "hh:mm").c_str())
        .arg(QString::fromStdString(prefixTags(sprintRef.tags())))
        .arg(QString::fromStdString(sprintRef.name()));
}

bool PomodoroModel::removeRows(int row, int count, const QModelIndex& index)
{
    beginRemoveRows(index, row, row + count - 1);
    remove(row);
    endRemoveRows();
    return true;
}

bool PomodoroModel::setData(const QModelIndex& index,
                            const QVariant& value,
                            int role)
{
    if (!index.isValid())
        return false;

    if (role == InsertRole) {
        if (index.row() != static_cast<int>(storage.size()) - 1)
            return false;
        storage[index.row()] = value.value<Sprint>();
        return true;
    }

    return false;
}

bool PomodoroModel::insertRows(int row, int count, const QModelIndex& parent)
{
    // Only support inserting one item after the last row
    if (row != static_cast<int>(storage.size()) || count != 1)
        return false;

    beginInsertRows(parent, rowCount(parent), rowCount(parent));
    storage.emplace_back(Sprint{});
    endInsertRows();
    return true;
}

QHash<int, QByteArray> PomodoroModel::roleNames() const
{
    QHash<int, QByteArray> result = QAbstractListModel::roleNames();
    result.insert(InsertRole, QByteArrayLiteral("insert"));
    result.insert(ItemRole, QByteArrayLiteral("item"));
    return result;
}

} // namespace sprint_timer::ui::qt_gui
