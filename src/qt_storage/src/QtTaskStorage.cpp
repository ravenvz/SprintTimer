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

auto QtTaskStorage::unfinishedTasks() -> std::vector<Task>
{
    return reader->unfinishedTasks();
}

auto QtTaskStorage::finishedTasks(const dw::DateRange& dateRange)
    -> std::vector<Task>
{
    return reader->finishedTasks(dateRange);
}

auto QtTaskStorage::allTasks(const dw::DateRange& dateRange)
    -> std::vector<Task>
{
    return reader->allTasks(dateRange);
}

auto QtTaskStorage::findByUuid(const std::string& uuid) -> std::vector<Task>
{
    return reader->findByUuid(uuid);
}

auto QtTaskStorage::findMatching(std::span<const std::string> uuids)
    -> std::vector<Task>
{
    return reader->findMatching(uuids);
}

auto QtTaskStorage::taskTree() -> TaskTree { return reader->taskTree(); }

auto QtTaskStorage::allTags() -> std::vector<std::string>
{
    return reader->allTags();
}

auto QtTaskStorage::save(const Task& task) -> void { writer->save(task); }

auto QtTaskStorage::saveTree(const TaskTree& taskTree) -> void
{
    writer->saveTree(taskTree);
}

auto QtTaskStorage::saveFullTree(const TaskTree& taskTree) -> void
{
    writer->saveFullTree(taskTree);
}

auto QtTaskStorage::remove(const std::string& uuid) -> void
{
    writer->remove(uuid);
}

auto QtTaskStorage::restore(const Task& task) -> void { writer->restore(task); }

auto QtTaskStorage::edit(const Task& oldTask, const Task& editedTask) -> void
{
    writer->edit(oldTask, editedTask);
}

auto QtTaskStorage::toggleCompleted(const std::string& uuid,
                                    const dw::DateTime& timeStamp) -> void
{
    writer->toggleCompleted(uuid, timeStamp);
}

auto QtTaskStorage::updatePriorities(const std::vector<std::string>& priorities)
    -> void
{
    writer->updatePriorities(priorities);
}

auto QtTaskStorage::editTag(const std::string& oldName,
                            const std::string& newName) -> void
{
    writer->editTag(oldName, newName);
}

} // namespace sprint_timer::storage::qt_storage
