/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "CommandLineTestRunner.h"
// #include "db_layer/db_service.h"
#include <vector>

// bool createTestDbConnection()
// {
//     DBService testDbService{":memory:"};
//     // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//     // db.setDatabaseName(":memory:");
//     // if (!db.open()) {
//     //     return false;
//     // }
//     // testDbService.createSchema();
//     // testDbService.activateForeignKeys();
//     return true;
// }

int main(int argc, char* argv[])
{
    std::vector<const char*> args(argv, argv + argc);
    args.push_back("-v");
    args.push_back("-c");

    // Create in-memory test database.
    // DBService testDbService{":memory:"};
    // if (!testDbService.createDbConnection()) {
    //     return 1;
    // };

    return CommandLineTestRunner::RunAllTests(argc, argv);
}
