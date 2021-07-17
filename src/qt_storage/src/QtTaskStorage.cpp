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
#include "qt_storage/QtTaskStorage.h"

namespace sprint_timer::storage::qt_storage {

QtTaskStorage::QtTaskStorage(std::unique_ptr<TaskStorageReader> reader_,
                             std::unique_ptr<TaskStorageWriter> writer_)
    : reader{std::move(reader_)}
    , writer{std::move(writer_)}
{
}

std::vector<entities::Task> QtTaskStorage::unfinishedTasks()
{
    return reader->unfinishedTasks();
}

std::vector<entities::Task>
QtTaskStorage::finishedTasks(const dw::DateRange& dateRange)
{
    return reader->finishedTasks(dateRange);
}

std::vector<entities::Task>
QtTaskStorage::allTasks(const dw::DateRange& dateRange)
{
    return reader->allTasks(dateRange);
}

std::vector<entities::Task> QtTaskStorage::findByUuid(const std::string& uuid)
{
    return reader->findByUuid(uuid);
}

std::vector<std::string> QtTaskStorage::allTags() { return reader->allTags(); }

void QtTaskStorage::save(const entities::Task& task) { writer->save(task); }

void QtTaskStorage::remove(const std::string& uuid) { writer->remove(uuid); }

void QtTaskStorage::edit(const entities::Task& oldTask,
                         const entities::Task& editedTask)
{
    writer->edit(oldTask, editedTask);
}

void QtTaskStorage::toggleCompleted(const std::string& uuid,
                                    const dw::DateTime& timeStamp)
{
    writer->toggleCompleted(uuid, timeStamp);
}

void QtTaskStorage::updatePriorities(const std::vector<std::string>& priorities)
{
    writer->updatePriorities(priorities);
}

void QtTaskStorage::editTag(const std::string& oldName,
                            const std::string& newName)
{
    writer->editTag(oldName, newName);
}

} // namespace sprint_timer::storage::qt_storage
