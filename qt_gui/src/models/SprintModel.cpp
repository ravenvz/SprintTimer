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

namespace sprint_timer::ui::qt_gui {

using dw::DateTime;
using dw::DateTimeRange;
using namespace entities;
using namespace sprint_timer::use_cases;

SprintModel::SprintModel(
    CommandHandler<DeleteSprintCommand>& deleteSprintHandler_,
    QueryHandler<RequestSprintsQuery, std::vector<Sprint>>&
        requestSprintsHandler_,
    DatasyncRelay& datasyncRelay_,
    QObject* parent_)
    : AsyncListModel(parent_)
    , deleteSprintHandler{deleteSprintHandler_}
    , requestSprintsHandler{requestSprintsHandler_}
    , datasyncRelay{datasyncRelay_}
{
    connect(&datasyncRelay_,
            &DatasyncRelay::dataUpdateRequiered,
            this,
            &AsyncListModel::requestSilentDataUpdate);
}

int SprintModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(storage.size());
}

QVariant SprintModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Sprint& sprintRef = storage[static_cast<size_t>(index.row())];

    switch (role) {
    case Qt::DisplayRole:
        return QString("%1 - %2 %3 %4")
            .arg(dw::to_string(sprintRef.startTime(), "hh:mm").c_str())
            .arg(dw::to_string(sprintRef.finishTime(), "hh:mm").c_str())
            .arg(QString::fromStdString(prefixTags(sprintRef.tags())))
            .arg(QString::fromStdString(sprintRef.name()));
    }

    return QVariant();
}

bool SprintModel::removeRows(int row, int count, const QModelIndex& index)
{
    beginRemoveRows(index, row, row + count - 1);
    remove(row);
    endRemoveRows();
    return true;
}

void SprintModel::remove(int row)
{
    deleteSprintHandler.handle(
        DeleteSprintCommand{storage[static_cast<size_t>(row)]});
    requestDataUpdate();
}

void SprintModel::requestUpdate()
{
    const dw::Date today{dw::current_date_local()};
    onDataChanged(requestSprintsHandler.handle(
        use_cases::RequestSprintsQuery{dw::DateRange{today, today}}));
}

void SprintModel::onDataChanged(const std::vector<Sprint>& items)
{
    datasyncRelay.onSyncCompleted("SprintModel");
    beginResetModel();
    storage = items;
    endResetModel();
}

} // namespace sprint_timer::ui::qt_gui
