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

#include "core/TaskStorageReader.h"
#include <QSqlQuery>
#include <queue>

namespace sprint_timer::storage::qt_storage {

class QtTaskStorageReader : public TaskStorageReader {
public:
    explicit QtTaskStorageReader(const QString& connectionName);

    std::vector<entities::Task> unfinishedTasks() final;

    std::vector<entities::Task>
    finishedTasks(const dw::DateRange& dateRange) final;

    std::vector<entities::Task> allTasks(const dw::DateRange& dateRange) final;

    std::vector<std::string> allTags() final;

private:
    const QString& connectionName;
    QSqlQuery finishedTasksQuery;
    QSqlQuery allTasksQuery;
    QSqlQuery tagsQuery;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTTASKSTORAGEREADER_H_48BDKQZ7 */
