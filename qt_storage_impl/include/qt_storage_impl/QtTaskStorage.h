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
#ifndef QTTASKSTORAGE_H_C1DPFISW
#define QTTASKSTORAGE_H_C1DPFISW

#include "qt_storage_impl/QtTaskStorageReader.h"
#include "qt_storage_impl/QtTaskStorageWriter.h"
#include <core/ITaskStorage.h>

namespace sprint_timer::storage::qt_storage_impl {

class QtTaskStorage : public ITaskStorage {
public:
    QtTaskStorage(std::unique_ptr<QtTaskStorageReader> reader,
                  std::unique_ptr<QtTaskStorageWriter> writer);

    void requestUnfinishedTasks(Handler handler) final;

    void requestFinishedTasks(const dw::DateRange& dateRange,
                              Handler handler) final;

    void requestTasks(const dw::DateRange& dateRange, Handler handler) final;

    void requestAllTags(TagHandler handler) final;

    void save(const entities::Task& task) final;

    void remove(const std::string& uuid) final;

    void edit(const entities::Task& task,
              const entities::Task& editedTask) final;

    void toggleTaskCompletionStatus(const std::string& uuid,
                                    const dw::DateTime& timeStamp) final;

    void updatePriorities(const std::vector<std::string>& priorities) final;

    void editTag(const std::string& oldName, const std::string& newName) final;

private:
    std::unique_ptr<QtTaskStorageReader> reader;
    std::unique_ptr<QtTaskStorageWriter> writer;
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: QTTASKSTORAGE_H_C1DPFISW */
