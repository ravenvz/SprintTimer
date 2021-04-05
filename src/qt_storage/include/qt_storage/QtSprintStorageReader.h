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
#ifndef QTSPRINTSTORAGEREADER_H_JXULCJ6I
#define QTSPRINTSTORAGEREADER_H_JXULCJ6I

#include "core/SprintStorageReader.h"
#include "core/entities/Tag.h"
#include <QSqlQuery>

namespace sprint_timer::storage::qt_storage {

class QtSprintStorageReader : public SprintStorageReader {
public:
    explicit QtSprintStorageReader(const QString& connectionName);

    QtSprintStorageReader(QtSprintStorageReader&&) = delete;
    QtSprintStorageReader& operator=(QtSprintStorageReader&&) = delete;

    QtSprintStorageReader(const QtSprintStorageReader&) = delete;
    QtSprintStorageReader& operator=(const QtSprintStorageReader&) = delete;

    std::vector<entities::Sprint>
    findByDateRange(const dw::DateRange& dateRange) final;

    std::vector<entities::Sprint> findByTaskUuid(const std::string& uuid) final;

private:
    const QString connectionName;
    QSqlQuery findByDateRangeQuery;
    QSqlQuery findByTaskUuidQuery;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTSPRINTSTORAGEREADER_H_JXULCJ6I */
