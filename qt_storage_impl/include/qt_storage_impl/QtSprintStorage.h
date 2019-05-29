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
#include "qt_storage_impl/DBService.h"
#include "qt_storage_impl/QtSprintStorageReader.h"
#include "qt_storage_impl/QtSprintStorageWriter.h"
#include <core/ISprintStorage.h>
#include <queue>

namespace sprint_timer::storage::qt_storage_impl {

class QtSprintStorage : public ISprintStorage {
public:
    QtSprintStorage(std::unique_ptr<ISprintStorageReader> reader,
                    std::unique_ptr<ISprintStorageWriter> writer);

    void requestItems(const dw::DateRange& dateRange, Handler handler) final;

    void sprintsForTask(const std::string& taskUuid, Handler handler) final;

    void save(const entities::Sprint& sprint) final;

    void save(const std::vector<entities::Sprint>& sprints) final;

    void remove(const entities::Sprint& sprint) final;

    void remove(const std::vector<entities::Sprint>& sprints) final;

private:
    std::unique_ptr<ISprintStorageReader> reader;
    std::unique_ptr<ISprintStorageWriter> writer;
};

} // namespace sprint_timer::storage::qt_storage_impl
