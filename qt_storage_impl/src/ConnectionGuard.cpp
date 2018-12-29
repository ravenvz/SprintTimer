#include "qt_storage_impl/ConnectionGuard.h"
#include <QSqlDatabase>

namespace sprint_timer::storage::qt_storage_impl {

ConnectionGuard::ConnectionGuard(const QString& dbName,
                                 const QString& connectionName)
    : connectionName{connectionName}
{
    // QSqlDriver* driver{new QSQLiteDriver{nullptr}};
    auto db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    if (dbName == "file::memory:?cache=shared") {
        db.setConnectOptions("QSQLITE_OPEN_URI");
    }
    db.setDatabaseName(dbName);
    if (!db.open()) {
        db.setConnectOptions();
    }
}

ConnectionGuard::~ConnectionGuard()
{
    QSqlDatabase::database(connectionName).close();
    QSqlDatabase::removeDatabase(connectionName);
}

} // namespace sprint_timer::storage::qt_storage_impl
