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
#ifndef QTTASKSTORAGEREADER_H_48BDKQZ7
#define QTTASKSTORAGEREADER_H_48BDKQZ7

#include "api/TaskStorageReader.h"
#include <QSqlQuery>
#include <queue>

namespace sprint_timer::storage::qt_storage {

class QtTaskStorageReader : public TaskStorageReader {
public:
    explicit QtTaskStorageReader(QString connectionName_);

    auto unfinishedTasks() -> std::vector<Task> final;

    auto finishedTasks(const dw::DateRange& dateRange)
        -> std::vector<Task> final;

    auto allTasks(const dw::DateRange& dateRange) -> std::vector<Task> final;

    auto allTags() -> std::vector<std::string> final;

    auto findByUuid(const std::string& uuid) -> std::vector<Task> final;

    auto findMatching(std::span<const std::string> uuids)
        -> std::vector<Task> final;

    auto taskTree() -> TaskTree final;

private:
    QString connectionName;
    QSqlQuery finishedTasksQuery;
    QSqlQuery allTasksQuery;
    QSqlQuery tagsQuery;
    QSqlQuery findByUuidQuery;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTTASKSTORAGEREADER_H_48BDKQZ7 */
