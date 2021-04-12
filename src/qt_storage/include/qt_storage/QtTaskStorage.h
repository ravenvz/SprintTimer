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

#include "qt_storage/QtTaskStorageReader.h"
#include "qt_storage/QtTaskStorageWriter.h"
#include "core/TaskStorage.h"

namespace sprint_timer::storage::qt_storage {

class QtTaskStorage : public TaskStorage {
public:
    QtTaskStorage(std::unique_ptr<TaskStorageReader> reader,
                  std::unique_ptr<TaskStorageWriter> writer);
    ~QtTaskStorage() override = default;

    QtTaskStorage(QtTaskStorage&&) = delete;
    QtTaskStorage& operator=(QtTaskStorage&&) = delete;

    QtTaskStorage(const QtTaskStorage&) = delete;
    QtTaskStorage& operator=(const QtTaskStorage&) = delete;

    std::vector<entities::Task> unfinishedTasks() final;

    std::vector<entities::Task>
    finishedTasks(const dw::DateRange& dateRange) final;

    std::vector<entities::Task> allTasks(const dw::DateRange& dateRange) final;

    std::vector<std::string> allTags() final;

    void save(const entities::Task& task) final;

    void remove(const std::string& uuid) final;

    void edit(const entities::Task& oldTask,
              const entities::Task& editedTask) final;

    void toggleCompleted(const std::string& uuid,
                         const dw::DateTime& timeStamp) final;

    void updatePriorities(const std::vector<std::string>& priorities) final;

    void editTag(const std::string& oldName, const std::string& newName) final;

private:
    std::unique_ptr<TaskStorageReader> reader;
    std::unique_ptr<TaskStorageWriter> writer;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTTASKSTORAGE_H_C1DPFISW */
