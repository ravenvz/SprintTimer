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
#include "qt_storage/QtSprintStorage.h"

namespace sprint_timer::storage::qt_storage {

QtSprintStorage::QtSprintStorage(std::unique_ptr<SprintStorageReader> reader_,
                                 std::unique_ptr<SprintStorageWriter> writer_)
    : reader{std::move(reader_)}
    , writer{std::move(writer_)}
{
}

std::vector<entities::Sprint>
QtSprintStorage::findByDateRange(const dw::DateRange& dateRange)
{
    return reader->findByDateRange(dateRange);
}

std::vector<entities::Sprint>
QtSprintStorage::findByTaskUuid(const std::string& taskUuid)
{
    return reader->findByTaskUuid(taskUuid);
}

std::vector<entities::Sprint>
QtSprintStorage::findByUuid(const std::string& uuid)
{
    return reader->findByUuid(uuid);
}

void QtSprintStorage::save(const entities::Sprint& sprint)
{
    writer->save(sprint);
}

void QtSprintStorage::save(const std::vector<entities::Sprint>& sprints)
{
    writer->save(sprints);
}

void QtSprintStorage::remove(const entities::Sprint& sprint)
{
    writer->remove(sprint);
}

void QtSprintStorage::remove(const std::vector<entities::Sprint>& sprints)
{
    writer->remove(sprints);
}

} // namespace sprint_timer::storage::qt_storage
