#include "qt_storage_impl/ConnectionGuard.h"
#include <QSqlDatabase>

namespace sprint_timer::storage::qt_storage_impl {

ConnectionGuard::ConnectionGuard(const QString& dbName,
                                 const QString& connectionName)
    : connectionName{connectionName}
{
    auto db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(dbName);
}

ConnectionGuard::~ConnectionGuard()
{
    QSqlDatabase::database(connectionName).close();
    QSqlDatabase::removeDatabase(connectionName);
}

} // namespace sprint_timer::storage::qt_storage_impl
