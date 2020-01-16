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
#include "qt_gui/models/OperationRangeModel.h"

namespace {

QStringList toDateStrings(const dw::DateRange& dateRange);

} // namespace

namespace sprint_timer::ui::qt_gui {

OperationRangeModel::OperationRangeModel(
    QueryHandler<use_cases::OperationalRangeQuery, dw::DateRange>&
        operRangeHandler_,
    DatasyncRelay& datasyncRelay_,
    QObject* parent_)
    : AsyncListModel{parent_}
    , operRangeHandler{operRangeHandler_}
    , datasyncRelay{datasyncRelay_}
{
    connect(&datasyncRelay_,
            &DatasyncRelay::dataUpdateRequiered,
            this,
            &AsyncListModel::requestSilentDataUpdate);
}

int OperationRangeModel::rowCount(const QModelIndex& parent) const
{
    return storage.size();
}

QVariant OperationRangeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant{};
    if (role != Qt::DisplayRole)
        return QVariant{};

    return storage[index.row()];
}

void OperationRangeModel::requestUpdate()
{
    const auto range =
        operRangeHandler.handle(use_cases::OperationalRangeQuery{});
    const int firstYear{static_cast<int>(range.start().year())};
    const int lastYear{static_cast<int>(range.finish().year())};
    datasyncRelay.onSyncCompleted("OperationRangeModel");
    if (storage.size() == lastYear - firstYear + 1)
        return;
    beginResetModel();
    storage = toDateStrings(range);
    endResetModel();
}

} // namespace sprint_timer::ui::qt_gui

namespace {

QStringList toDateStrings(const dw::DateRange& dateRange)
{
    QStringList dates;
    int startYear{static_cast<int>(dateRange.start().year())};
    const int endYear{static_cast<int>(dateRange.finish().year())};
    for (int current = startYear; current <= endYear; ++current) {
        dates.push_back(QString{"%1"}.arg(current));
    }
    return dates;
}

} // namespace
