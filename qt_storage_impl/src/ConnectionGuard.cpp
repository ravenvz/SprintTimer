#include "qt_storage_impl/ConnectionGuard.h"
#include "qt_storage_impl/DatabaseError.h"
#include <QSqlDatabase>

namespace sprint_timer::storage::qt_storage_impl {

ConnectionGuard::ConnectionGuard(const QString& dbName,
                                 const QString& connectionName)
    : connectionName{connectionName}
{
    auto db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    if (dbName == "file::memory:?cache=shared") {
        db.setConnectOptions("QSQLITE_OPEN_URI");
    }
    db.setDatabaseName(dbName);
    if (!db.open() || db.isOpenError())
        throw DatabaseError{"Unable to open connection to the database", db};
}

ConnectionGuard::~ConnectionGuard()
{
    QSqlDatabase::database(connectionName).close();
    QSqlDatabase::removeDatabase(connectionName);
}

} // namespace sprint_timer::storage::qt_storage_impl
