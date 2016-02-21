#include "db_layer/db_helper.h"
#include "CommandLineTestRunner.h"
#include <vector>

bool createTestDbConnection() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        return false;
    }
    createSchema();
    activateForeignKeys();
    return true;
}

int main(int argc, char* argv[]) {
    std::vector<const char*> args(argv, argv + argc);
    args.push_back("-v");
    args.push_back("-c");

    // Create in-memory test database.
    if (!createTestDbConnection()) {
        return 1;
    };

    return CommandLineTestRunner::RunAllTests(argc, argv);
}
