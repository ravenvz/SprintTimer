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
#include "qt_gui/models/TagModel.h"
#include <core/use_cases/RenameTag.h>
#include <core/use_cases/RequestAllTags.h>

namespace sprint_timer::ui::qt_gui {

using use_cases::RenameTag;
using use_cases::RequestAllTags;

TagModel::TagModel(ITaskStorageReader& taskReader,
                   ITaskStorageWriter& taskWriter,
                   CommandInvoker& commandInvoker,
                   QueryExecutor& queryExecutor,
                   QObject* parent)
    : AsyncListModel{parent}
    , taskReader{taskReader}
    , taskWriter{taskWriter}
    , commandInvoker{commandInvoker}
    , queryExecutor{queryExecutor}
{
    synchronize();
}

Qt::ItemFlags TagModel::flags(const QModelIndex& index) const
{
    return AsyncListModel::flags(index) | Qt::ItemIsEditable;
}

int TagModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(storage.size());
}

QVariant TagModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const std::string tag = storage[static_cast<size_t>(index.row())];

    switch (role) {
    case Qt::DisplayRole:
        return QString::fromStdString(tag);
    case Qt::EditRole:
        return QString::fromStdString(tag);
    default:
        return QVariant();
    }
}

bool TagModel::setData(const QModelIndex& index,
                       const QVariant& value,
                       int role)
{
    if (!index.isValid())
        return false;
    if (role == Qt::EditRole) {
        buffer.push_back({data(index, role).toString().toStdString(),
                          value.toString().toStdString()});
        storage[static_cast<size_t>(index.row())]
            = value.toString().toStdString();
        return true;
    }
    return false;
}

void TagModel::submitData()
{
    while (!buffer.empty()) {
        commandInvoker.executeCommand(std::make_unique<RenameTag>(
            taskWriter, buffer.back().first, buffer.back().second));
        buffer.pop_back();
    }
    requestDataUpdate();
}

void TagModel::revertData()
{
    buffer.clear();
    requestDataUpdate();
}

void TagModel::requestDataUpdate()
{
    queryExecutor.executeQuery(std::make_unique<RequestAllTags>(
        taskReader, [this](const auto& tags) { this->onDataArrived(tags); }));
}

void TagModel::onDataArrived(const std::vector<std::string>& tags)
{
    beginResetModel();
    storage = tags;
    endResetModel();
    broadcastUpdateFinished();
}

} // namespace sprint_timer::ui::qt_gui
