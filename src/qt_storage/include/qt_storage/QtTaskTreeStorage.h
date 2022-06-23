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
#ifndef TASKTREEFILESTORAGE_H_YQ94RXZP
#define TASKTREEFILESTORAGE_H_YQ94RXZP

#include "core/TaskTreeMetadataStorage.h"
#include <QSqlQuery>
#include <filesystem>

namespace sprint_timer::storage::qt_storage {

class QtTaskTreeStorage : public TaskTreeMetadataStorage {
public:
    QtTaskTreeStorage(std::unique_ptr<TaskTreeMetadataReader> reader,
                      std::unique_ptr<TaskTreeMetadataWriter> writer);

    [[nodiscard]] TaskMetadataTree readTree() const override;

    void saveTree(const TaskMetadataTree& taskTree) const override;

private:
    std::unique_ptr<TaskTreeMetadataReader> reader;
    std::unique_ptr<TaskTreeMetadataWriter> writer;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: TASKTREEFILESTORAGE_H_YQ94RXZP */
