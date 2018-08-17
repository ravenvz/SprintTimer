/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include <core/use_cases/IncrementTaskSprints.h>
#include <core/use_cases/RemoveSprintTransaction.h>
#include <core/use_cases/RequestSprints.h>

namespace sprint_timer::ui::qt_gui {

using dw::DateTime;
using dw::TimeSpan;
using namespace entities;
using namespace sprint_timer::use_cases;

SprintModel::SprintModel(ICoreService& coreService,
                         CommandInvoker& commandInvoker,
                         QueryExecutor& queryExecutor,
                         ISprintStorageReader& sprintReader,
                         ISprintStorageWriter& sprintWriter,
                         ITaskStorageWriter& taskWriter,
                         QObject* parent)
    : AsyncListModel(parent)
    , coreService{coreService}
    , commandInvoker{commandInvoker}
    , queryExecutor{queryExecutor}
    , sprintReader{sprintReader}
    , sprintWriter{sprintWriter}
    , taskWriter{taskWriter}
{
    synchronize();
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
            .arg(sprintRef.startTime().toString("hh:mm").c_str())
            .arg(sprintRef.finishTime().toString("hh:mm").c_str())
            .arg(QString::fromStdString(prefixTags(sprintRef.tags())))
            .arg(QString::fromStdString(sprintRef.name()));
    }

    return QVariant();
}

void SprintModel::insert(const TimeSpan& timeSpan, const std::string& taskUuid)
{
    coreService.registerSprint(timeSpan, taskUuid);
    requestDataUpdate();
}

void SprintModel::insert(const Sprint& sprint)
{
    coreService.registerSprint(sprint);
    requestDataUpdate();
}

void SprintModel::insert(const std::vector<Sprint>& sprints)
{
    for (const auto& sprint : sprints)
        coreService.registerSprint(sprint);
    requestDataUpdate();
}

void SprintModel::remove(int row)
{
    coreService.removeSprint(storage[static_cast<size_t>(row)]);
    requestDataUpdate();
}

void SprintModel::requestDataUpdate()
{
    queryExecutor.executeQuery(std::make_unique<RequestSprints>(
        sprintReader,
        dw::TimeSpan{dw::DateTime::currentDateTimeLocal(),
                     dw::DateTime::currentDateTimeLocal()},
        [this](const auto& items) { this->onDataChanged(items); }));
}

void SprintModel::onDataChanged(const std::vector<Sprint>& items)
{
    beginResetModel();
    storage = items;
    endResetModel();
    broadcastUpdateFinished();
}

} // namespace sprint_timer::ui::qt_gui
