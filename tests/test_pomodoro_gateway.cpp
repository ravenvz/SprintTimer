#include "data_base/db_helper.h"
#include "core/entities.h"
#include <TestHarness.h>

bool createTestDbConnection() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        return false;
    }
    return true;
}

TEST_GROUP(PomodoroGatewayGroup) {

    // void setup() {
        // createDbConnection();
    // }

    // void teardown() {
        // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        // db.close();
        // QSqlDatabase::removeDatabase("QSQLITE");
    // }

};

TEST(PomodoroGatewayGroup, test_name) {
    // createTestDbConnection();
    // QDateTime start = QDateTime::currentDateTime();
    // QDateTime finish = QDateTime::currentDateTime();
    // Pomodoro pomodoro {"TestPomo1", start, finish};
    // PomodoroGateway::storePomodoro(pomodoro);
    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // db.close();
    // QSqlDatabase::removeDatabase("QSQLITE");
    
    
    
}
