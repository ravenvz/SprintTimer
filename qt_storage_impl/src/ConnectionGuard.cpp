#include "qt_storage_impl/ConnectionGuard.h"
#include <QSqlDatabase>

#include <iostream>

namespace sprint_timer::storage::qt_storage_impl {

ConnectionGuard::ConnectionGuard(const QString& dbName,
                                 const QString& connectionName)
    : connectionName{connectionName}
{
    auto db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(dbName);
	if (dbName == ":memory:") {
		std::cout << "Opening with shared cache option" << std::endl;
		db.setConnectOptions("QSQLITE_ENABLE_SHARED_CACHE");
	}
	QSqlDatabase::database(connectionName).open();
	std::cout << "Connection created: " << connectionName.toStdString() << std::endl;
}

ConnectionGuard::~ConnectionGuard()
{
    QSqlDatabase::database(connectionName).close();
    QSqlDatabase::removeDatabase(connectionName);
	std::cout << "Connection destroyed: " << connectionName.toStdString() << std::endl;
}

} // namespace sprint_timer::storage::qt_storage_impl
