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
#include "models/PomodoroModel.h"

PomodoroModel::PomodoroModel(IPomodoroService& pomodoroService, QObject* parent)
    : AsyncListModel(parent)
    , interval{TimeSpan{DateTime::currentDateTime(),
                        DateTime::currentDateTime()}}
    , pomodoroService{pomodoroService}
{
    synchronize();
}

int PomodoroModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(storage.size());
}

QVariant PomodoroModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Sprint pomodoro = storage[static_cast<size_t>(index.row())];

    switch (role) {
    case Qt::DisplayRole:
        return QString::fromStdString(pomodoro.toString());
    }

    return QVariant();
}

void PomodoroModel::setDateFilter(const TimeSpan& timeSpan)
{
    interval = timeSpan;
    requestDataUpdate();
}

void PomodoroModel::insert(const TimeSpan& timeSpan,
                           const std::string& taskUuid)
{
    pomodoroService.registerPomodoro(timeSpan, taskUuid);
    requestDataUpdate();
}

void PomodoroModel::remove(int row)
{
    pomodoroService.removePomodoro(storage[static_cast<size_t>(row)]);
    requestDataUpdate();
}

void PomodoroModel::requestDataUpdate()
{
    pomodoroService.pomodorosInTimeRange(
        interval,
        std::bind(&PomodoroModel::onDataChanged, this, std::placeholders::_1));
}

void PomodoroModel::onDataChanged(const std::vector<Sprint>& items)
{
    beginResetModel();
    storage = items;
    endResetModel();
    broadcastUpdateFinished();
}
