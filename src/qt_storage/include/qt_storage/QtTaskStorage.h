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
#ifndef QTTASKSTORAGE_H_C1DPFISW
#define QTTASKSTORAGE_H_C1DPFISW

#include "api/TaskStorage.h"
#include "qt_storage/QtTaskStorageReader.h"
#include "qt_storage/QtTaskStorageWriter.h"

namespace sprint_timer::storage::qt_storage {

class QtTaskStorage : public api::TaskStorage {
public:
    QtTaskStorage(std::unique_ptr<api::TaskStorageReader> reader,
                  std::unique_ptr<api::TaskStorageWriter> writer);
    ~QtTaskStorage() override = default;

    QtTaskStorage(QtTaskStorage&&) = delete;
    QtTaskStorage& operator=(QtTaskStorage&&) = delete;

    QtTaskStorage(const QtTaskStorage&) = delete;
    QtTaskStorage& operator=(const QtTaskStorage&) = delete;

    auto unfinishedTasks() -> std::vector<Task> final;

    auto
    finishedTasks(const dw::DateRange& dateRange) -> std::vector<Task> final;

    auto allTasks(const dw::DateRange& dateRange) -> std::vector<Task> final;

    auto allTags() -> std::vector<std::string> final;

    auto findByUuid(const std::string& uuid) -> std::vector<Task> final;

    auto
    findMatching(std::span<const std::string> uuids) -> std::vector<Task> final;

    auto taskTree() -> TaskTree final;

    auto save(const Task& task) -> void final;

    auto saveTree(const TaskTree& taskTree) -> void final;

    auto saveFullTree(const TaskTree& taskTree) -> void final;

    auto remove(const std::string& uuid) -> void final;

    auto restore(const Task& task) -> void final;

    auto edit(const Task& oldTask, const Task& editedTask) -> void final;

    auto toggleCompleted(const std::string& uuid,
                         const dw::DateTime& timeStamp) -> void final;

    auto
    updatePriorities(const std::vector<std::string>& priorities) -> void final;

    auto editTag(const std::string& oldName,
                 const std::string& newName) -> void final;

private:
    std::unique_ptr<TaskStorageReader> reader;
    std::unique_ptr<TaskStorageWriter> writer;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTTASKSTORAGE_H_C1DPFISW */
