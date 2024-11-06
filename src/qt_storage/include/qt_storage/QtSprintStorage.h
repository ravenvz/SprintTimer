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

#include "api/SprintStorage.h"
#include "qt_storage/QtSprintStorageReader.h"
#include "qt_storage/QtSprintStorageWriter.h"

namespace sprint_timer::storage::qt_storage {

class QtSprintStorage : public api::SprintStorage {
public:
    QtSprintStorage(std::unique_ptr<SprintStorageReader> reader,
                    std::unique_ptr<SprintStorageWriter> writer);
    ~QtSprintStorage() override = default;

    QtSprintStorage(QtSprintStorage&&) = delete;
    QtSprintStorage& operator=(QtSprintStorage&&) = delete;

    QtSprintStorage(const QtSprintStorage&) = delete;
    QtSprintStorage& operator=(const QtSprintStorage&) = delete;

    std::vector<SprintRecord>
    findByDateRange(const dw::DateRange& dateRange) final;

    void save(const std::string& taskUuid,
              std::span<const Sprint> sprints) final;

    void remove(const Sprint& sprint) final;

    void remove(std::span<const Sprint> sprints) final;

    void restore(const Sprint& sprint) final;

private:
    std::unique_ptr<SprintStorageReader> reader;
    std::unique_ptr<SprintStorageWriter> writer;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTSPRINTSTORAGE_H_HNDKFVAS */
