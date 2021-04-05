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
#ifndef QTSPRINTSTORAGEWRITER_H_U7AAXVTC
#define QTSPRINTSTORAGEWRITER_H_U7AAXVTC

#include <QSqlQuery>
#include <core/SprintStorageWriter.h>

namespace sprint_timer::storage::qt_storage {

class QtSprintStorageWriter : public SprintStorageWriter {
public:
    explicit QtSprintStorageWriter(const QString& connectionName);

    QtSprintStorageWriter(QtSprintStorageWriter&&) = delete;
    QtSprintStorageWriter& operator=(QtSprintStorageWriter&&) = delete;

    QtSprintStorageWriter(const QtSprintStorageWriter&) = delete;
    QtSprintStorageWriter& operator=(const QtSprintStorageWriter&) = delete;

    void save(const entities::Sprint& sprint) final;

    void save(const std::vector<entities::Sprint>& sprints) final;

    void remove(const entities::Sprint& sprint) final;

    void remove(const std::vector<entities::Sprint>& sprints) final;

private:
    const QString& connectionName;
    QSqlQuery saveSprintQuery;
    QSqlQuery deleteSprintQuery;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTSPRINTSTORAGEWRITER_H_U7AAXVTC */
