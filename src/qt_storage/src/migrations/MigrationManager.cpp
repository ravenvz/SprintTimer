/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "qt_storage/migrations/MigrationManager.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/QueryError.h"
#include "qt_storage/TransactionGuard.h"
#include "qt_storage/utils/QueryUtils.h"
#include <QDebug>
#include <QFile>
#include <QStringBuilder>

namespace {

using namespace sprint_timer::storage::qt_storage;

void updateVersion(const QString& connectionName, unsigned newVersion);

unsigned getDatabaseVersion(const QString& connectionName);

class Backup {
public:
    explicit Backup(const QString& filename);

    Backup(Backup&&) = delete;
    Backup& operator=(Backup&&) = delete;

    Backup(Backup&) = delete;
    Backup& operator=(Backup&) = delete;

    ~Backup();

    void release();

private:
    const QString filename;
    bool shouldRestore{false};
};

void createBackupCopy(const QString& filename);

void restoreBackupCopy(const QString& filename);

const QString getVersionQuery{"SELECT " % InfoTable::Columns::value % " from " %
                              InfoTable::name % " where " %
                              InfoTable::Columns::name % " = (:name)"};

} // namespace

namespace sprint_timer::storage::qt_storage {

MigrationManager::MigrationManager(unsigned currentDbVersion)
    : currentDatabaseVersion{currentDbVersion}
{
}

void MigrationManager::runMigrations(const QString& connectionName) const
{
    auto version = getDatabaseVersion(connectionName);
    qDebug() << "Latest database version is " << currentDatabaseVersion;
    qDebug() << "Actual database version is " << version;

    if (version == currentDatabaseVersion)
        return;

    Backup backupController{
        QSqlDatabase::database(connectionName).databaseName()};

    // TODO disable foreign keys here

    while (version < currentDatabaseVersion) {
        if (auto migrationFound = migrations.find(version);
            migrationFound != migrations.end()) {
            qDebug() << "Running migration from version " << version
                     << " to version " << version + 1 << "...";
            TransactionGuard transactionGuard{connectionName};
            migrationFound->second->run(connectionName);
            updateVersion(connectionName, ++version);
            transactionGuard.commit();
            qDebug() << "Migration successfull";
        }
        else {
            throw std::runtime_error{
                std::string{"Could not find migration from version"} +
                std::to_string(version)};
        }
    }
    backupController.release();
}

void MigrationManager::addMigration(unsigned version,
                                    std::unique_ptr<Migration> migration)
{
    migrations.insert({version, std::move(migration)});
}

} // namespace sprint_timer::storage::qt_storage

namespace {

Backup::Backup(const QString& filename)
    : filename{filename}
{
    createBackupCopy(filename);
    shouldRestore = true;
}

Backup::~Backup()
{
    if (shouldRestore) {
        try {
            restoreBackupCopy(filename);
        }
        catch (std::runtime_error& err) {
            qDebug() << err.what();
        }
    }
}

void Backup::release() { shouldRestore = false; }

void createBackupCopy(const QString& filename)
{
    bool ok{true};
    if (QFile::exists(filename + ".old"))
        ok = ok && QFile::remove(filename + ".old");
    if (!ok || !QFile::copy(filename, filename + ".old"))
        throw std::runtime_error{std::string{"Error creating backup from "} +
                                 filename.toStdString() + " to " +
                                 filename.toStdString() + ".old"};
    qDebug() << "Created backup copy of the database";
}

void restoreBackupCopy(const QString& filename)
{
    bool ok{true};
    if (QFile::exists(filename))
        ok = ok && QFile::remove(filename);
    if (!ok || !QFile::copy(filename + ".old", filename))
        throw std::runtime_error{std::string{"Error restoring backup from "} +
                                 filename.toStdString() + ".old to " +
                                 filename.toStdString()};
    qDebug() << "Restored database from backup copy";
}

unsigned getDatabaseVersion(const QString& connectionName)
{
    using namespace sprint_timer::storage::qt_storage;
    QSqlQuery query = tryPrepare(connectionName, getVersionQuery);
    query.bindValue(":name", "version");
    tryExecute(query);

    if (!query.first())
        throw QueryError{"Error querying for database version", query};

    const QString versionStr{query.value(0).toString()};
    auto version = versionStr.toUInt();
    query.finish();

    return version;
}

void updateVersion(const QString& connectionName, unsigned newVersion)
{
    const QString queryStr =
        QString{"update %1 set %2 = (:value) where %3 = (:name)"}
            .arg(InfoTable::name)
            .arg(InfoTable::Columns::value)
            .arg(InfoTable::Columns::name);
    QSqlQuery query = tryPrepare(connectionName, queryStr);
    query.bindValue(":name", "version");
    query.bindValue(":value", QString{"%1"}.arg(newVersion));
    tryExecute(query);
}

} // namespace
