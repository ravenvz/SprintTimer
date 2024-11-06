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
#ifndef QTSPRINTSTORAGEWRITER_H_U7AAXVTC
#define QTSPRINTSTORAGEWRITER_H_U7AAXVTC

#include "api/SprintStorageWriter.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include <QSqlQuery>

namespace sprint_timer::storage::qt_storage {

class QtSprintStorageWriter : public api::SprintStorageWriter {
public:
    explicit QtSprintStorageWriter(QString connectionName);

    QtSprintStorageWriter(QtSprintStorageWriter&&) = delete;
    QtSprintStorageWriter& operator=(QtSprintStorageWriter&&) = delete;

    QtSprintStorageWriter(const QtSprintStorageWriter&) = delete;
    QtSprintStorageWriter& operator=(const QtSprintStorageWriter&) = delete;

    void save(const std::string& taskUuid,
              std::span<const Sprint> sprints) final;

    void remove(const Sprint& sprint) final;

    void remove(std::span<const Sprint> sprints) final;

    void restore(const Sprint& sprint) final;

private:
    QString connectionName;
    QSqlQuery saveSprintQuery;
    QSqlQuery deleteSprintQuery;
    QSqlQuery restoreSprintQuery;
    utils::DateTimeConverter dateTimeConverter;

    void save(const std::string& taskUuid, const Sprint& sprint);
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTSPRINTSTORAGEWRITER_H_U7AAXVTC */
