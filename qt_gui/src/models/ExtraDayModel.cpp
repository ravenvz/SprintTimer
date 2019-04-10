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

void ExtraDayModel::setExtraDayData(const std::vector<dw::Date>& days)
{
    beginResetModel();
    data_ = days;
    std::sort(data_.begin(), data_.end());
    endResetModel();
}

int ExtraDayModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(data_.size());
}

bool ExtraDayModel::removeRows(int row, int count, const QModelIndex& index)
{
    beginRemoveRows(QModelIndex(), row, row + count);
    data_.erase(data_.begin() + row, data_.begin() + row + count);
    endRemoveRows();
    return true;
}

QVariant ExtraDayModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto& date = data_[static_cast<size_t>(index.row())];

    switch (role) {
    case Qt::DisplayRole:
        return QString::fromStdString(dw::to_string(date, "dd.MM.yyyy"));
    default:
        return QVariant();
    }
}

void ExtraDayModel::insert(const dw::Date& date)
{
    beginInsertRows(QModelIndex(), rowCount(), 1);
    data_.push_back(date);
    endInsertRows();
}

dw::Date ExtraDayModel::dateAt(int row) { return data_[row]; }

} // namespace sprint_timer::ui::qt_gui
