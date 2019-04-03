/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#ifndef MIGRATIONMANAGER_H_PRWBSWIP
#define MIGRATIONMANAGER_H_PRWBSWIP

#include "qt_storage_impl/migrations/Migration.h"
#include <memory>
#include <unordered_map>

namespace sprint_timer::storage::qt_storage_impl {

class MigrationManager {
public:
    MigrationManager(unsigned currentDbVersion);

    void runMigrations(QSqlDatabase& database) const;

    void addMigration(unsigned version, std::unique_ptr<Migration> migration);

private:
    unsigned currentDatabaseVersion;
    std::unordered_map<unsigned, std::unique_ptr<Migration>> migrations;
};


} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: MIGRATIONMANAGER_H_PRWBSWIP */
