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
