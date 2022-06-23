/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "qt_storage/QtTaskTreeStorage.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/utils/QueryUtils.h"
#include <utility>

namespace {

enum class AdvColumn {
    Name,
    Uuid,
    EstimatedCost,
    Tags,
    Completed,
    StartTime,
    FinishTime,
    Priority,
    LastModified,
    SprintUuid
};

} // namespace

namespace sprint_timer::storage::qt_storage {

QtTaskTreeStorage::QtTaskTreeStorage(
    std::unique_ptr<TaskTreeMetadataReader> reader_,
    std::unique_ptr<TaskTreeMetadataWriter> writer_)
    : reader{std::move(reader_)}
    , writer{std::move(writer_)}
{
}

TaskMetadataTree QtTaskTreeStorage::readTree() const
{
    return reader->readTree();
}

void QtTaskTreeStorage::saveTree(const TaskMetadataTree& taskTree) const
{
    writer->saveTree(taskTree);
}

} // namespace sprint_timer::storage::qt_storage
