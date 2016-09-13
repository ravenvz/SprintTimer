/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "models/SprintModel.h"

SprintModel::SprintModel(ICoreService& coreService, QObject* parent)
    : AsyncListModel(parent)
    , interval{TimeSpan{DateTime::currentDateTime(),
                        DateTime::currentDateTime()}}
    , coreService{coreService}
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

void SprintModel::setDateFilter(const TimeSpan& timeSpan)
{
    interval = timeSpan;
    requestDataUpdate();
}

void SprintModel::insert(const TimeSpan& timeSpan, const std::string& taskUuid)
{
    coreService.registerSprint(timeSpan, taskUuid);
    requestDataUpdate();
}

void SprintModel::remove(int row)
{
    coreService.removeSprint(storage[static_cast<size_t>(row)]);
    requestDataUpdate();
}

void SprintModel::requestDataUpdate()
{
    coreService.sprintsInTimeRange(
        interval,
        std::bind(&SprintModel::onDataChanged, this, std::placeholders::_1));
}

void SprintModel::onDataChanged(const std::vector<Sprint>& items)
{
    beginResetModel();
    storage = items;
    endResetModel();
    broadcastUpdateFinished();
}
