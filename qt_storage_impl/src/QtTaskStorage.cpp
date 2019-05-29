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
#include "qt_storage_impl/QtTaskStorage.h"

namespace sprint_timer::storage::qt_storage_impl {

QtTaskStorage::QtTaskStorage(std::unique_ptr<QtTaskStorageReader> reader_,
                             std::unique_ptr<QtTaskStorageWriter> writer_)
    : reader{std::move(reader_)}
    , writer{std::move(writer_)}
{
}

void QtTaskStorage::requestUnfinishedTasks(Handler handler)
{
    reader->requestUnfinishedTasks(handler);
}

void QtTaskStorage::requestFinishedTasks(const dw::DateRange& dateRange,
                                         Handler handler)
{
    reader->requestFinishedTasks(dateRange, handler);
}

void QtTaskStorage::requestTasks(const dw::DateRange& dateRange,
                                 Handler handler)
{
    reader->requestTasks(dateRange, handler);
}

void QtTaskStorage::requestAllTags(TagHandler handler)
{
    reader->requestAllTags(handler);
}

void QtTaskStorage::save(const entities::Task& task) { writer->save(task); }

void QtTaskStorage::remove(const std::string& uuid) { writer->remove(uuid); }

void QtTaskStorage::edit(const entities::Task& task,
                         const entities::Task& editedTask)
{
    writer->edit(task, editedTask);
}

void QtTaskStorage::QtTaskStorage::toggleTaskCompletionStatus(
    const std::string& uuid, const dw::DateTime& timeStamp)
{
    writer->toggleTaskCompletionStatus(uuid, timeStamp);
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

} // namespace sprint_timer::storage::qt_storage_impl
