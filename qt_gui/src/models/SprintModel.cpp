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
#include <core/use_cases/RegisterNewSprint.h>
#include <core/use_cases/RegisterNewSprintBulk.h>
#include <core/use_cases/RemoveSprintTransaction.h>
#include <core/use_cases/RequestSprints.h>

namespace sprint_timer::ui::qt_gui {

using dw::DateTime;
using dw::DateTimeRange;
using namespace entities;
using namespace sprint_timer::use_cases;

SprintModel::SprintModel(CommandInvoker& commandInvoker_,
                         QueryInvoker& queryInvoker_,
                         ISprintStorage& sprintStorage_,
                         QObject* parent_)
    : AsyncListModel(parent_)
    , commandInvoker{commandInvoker_}
    , queryInvoker{queryInvoker_}
    , sprintStorage{sprintStorage_}
{
    requestSilentDataUpdate();
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

void SprintModel::insert(const DateTimeRange& timeSpan,
                         const std::string& taskUuid)
{
    Sprint sprint{taskUuid, timeSpan};
    insert(sprint);
}

void SprintModel::insert(const Sprint& sprint)
{
    registerSprint(sprint);
    requestDataUpdate();
}

void SprintModel::insert(const std::vector<Sprint>& sprints)
{
    commandInvoker.executeCommand(
        std::make_unique<RegisterNewSprintBulk>(sprintStorage, sprints));
    requestDataUpdate();
}

void SprintModel::registerSprint(const Sprint& sprint)
{
    commandInvoker.executeCommand(
        std::make_unique<RegisterNewSprint>(sprintStorage, sprint));
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
    commandInvoker.executeCommand(std::make_unique<RemoveSprintTransaction>(
        sprintStorage, storage[static_cast<size_t>(row)]));
    requestDataUpdate();
}

const Sprint& SprintModel::itemAt(int row) const { return storage[row]; }

void SprintModel::requestUpdate(const dw::DateRange& dateRange)
{
    sprintDateRange = dateRange;
    requestUpdate();
}

void SprintModel::requestUpdate()
{
    queryInvoker.execute(std::make_unique<RequestSprints>(
        sprintStorage, sprintDateRange, [this](const auto& items) {
            onDataChanged(items);
        }));
}

void SprintModel::onDataChanged(const std::vector<Sprint>& items)
{
    beginResetModel();
    storage = items;
    endResetModel();
}

std::vector<Sprint> allSprints(const SprintModel& sprintModel)
{
    std::vector<Sprint> sprints;
    const int numRows{sprintModel.rowCount(QModelIndex{})};
    sprints.reserve(numRows);
    for (int row = 0; row < numRows; ++row)
        sprints.push_back(sprintModel.itemAt(row));
    return sprints;
}

} // namespace sprint_timer::ui::qt_gui
