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
#include "qt_gui/models/ExtraDayModel.h"

namespace sprint_timer::ui::qt_gui {

ExtraDayModel::ExtraDayModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

int ExtraDayModel::rowCount(const QModelIndex& /*parent*/) const
{
    return static_cast<int>(data_.size());
}

bool ExtraDayModel::insertRows(int row, int count, const QModelIndex& index)
{
    if (count <= 0 || row < 0)
        return false;
    beginInsertRows(index, row, row + count - 1);
    data_.insert(data_.begin() + row, static_cast<size_t>(count), {QDate(), 0});
    endInsertRows();
    return true;
}

bool ExtraDayModel::removeRows(int row, int count, const QModelIndex& index)
{
    if (count <= 0 || row < 0 || (row + count) > rowCount(index))
        return false;
    beginRemoveRows(index, row, row + count - 1);
    data_.erase(data_.begin() + row, data_.begin() + row + count);
    endRemoveRows();
    return true;
}

QVariant ExtraDayModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto& [date, goal] = data_[static_cast<size_t>(index.row())];
    const auto& entry = data_[static_cast<size_t>(index.row())];

    switch (role) {
    case Qt::DisplayRole:
        return QString{"%1 %2"}.arg(date.toString("dd.MM.yyyy")).arg(goal);
    case Qt::EditRole: {
        QVariant ventry;
        ventry.setValue(entry);
        return ventry;
    }
    default:
        return QVariant();
    }
}

bool ExtraDayModel::setData(const QModelIndex& index,
                            const QVariant& data,
                            int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;
    data_[static_cast<size_t>(index.row())] = data.value<QPair<QDate, int>>();
    QVector<int> roles;
    roles << role;
    emit dataChanged(index, index, roles);
    return true;
}

void ExtraDayModel::sort(int /*column*/, Qt::SortOrder order)
{
    emit layoutAboutToBeChanged();
    if (order == Qt::AscendingOrder)
        std::sort(data_.begin(), data_.end());
    else
        std::sort(
            data_.begin(), data_.end(), [](const auto& lhs, const auto& rhs) {
                return lhs.first > rhs.first;
            });
    emit layoutChanged();
}

} // namespace sprint_timer::ui::qt_gui
