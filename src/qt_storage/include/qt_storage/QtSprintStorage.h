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
#ifndef QTSPRINTSTORAGE_H_HNDKFVAS
#define QTSPRINTSTORAGE_H_HNDKFVAS

#include "core/SprintStorage.h"
#include "qt_storage/QtSprintStorageReader.h"
#include "qt_storage/QtSprintStorageWriter.h"

namespace sprint_timer::storage::qt_storage {

class QtSprintStorage : public SprintStorage {
public:
    QtSprintStorage(std::unique_ptr<SprintStorageReader> reader,
                    std::unique_ptr<SprintStorageWriter> writer);
    ~QtSprintStorage() override = default;

    QtSprintStorage(QtSprintStorage&&) = delete;
    QtSprintStorage& operator=(QtSprintStorage&&) = delete;

    QtSprintStorage(const QtSprintStorage&) = delete;
    QtSprintStorage& operator=(const QtSprintStorage&) = delete;

    std::vector<entities::Sprint>
    findByDateRange(const dw::DateRange& dateRange) final;

    std::vector<entities::Sprint>
    findByTaskUuid(const std::string& taskUuid) final;

    std::vector<entities::Sprint> findByUuid(const std::string& uuid) final;

    void save(const entities::Sprint& sprint) final;

    void save(const std::vector<entities::Sprint>& sprints) final;

    void remove(const entities::Sprint& sprint) final;

    void remove(const std::vector<entities::Sprint>& sprints) final;

private:
    std::unique_ptr<SprintStorageReader> reader;
    std::unique_ptr<SprintStorageWriter> writer;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTSPRINTSTORAGE_H_HNDKFVAS */
