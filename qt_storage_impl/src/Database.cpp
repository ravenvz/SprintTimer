/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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

#include "qt_storage_impl/Database.h"

#include <QDebug>
#include <QSqlError>
#include <QtCore/QFile>

namespace {
    unsigned currentDatabaseVersion{3};
}

Database::Database(const QString& filename)
        : filename{filename}
{
    if (!prepare())
        throw std::runtime_error("Cannot find or create database");
}

bool Database::prepare()
{
    bool ok{true};
    {
        auto db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(filename);
        QFile databaseFile{filename};

        if (!databaseFile.exists()) {
            ok = ok && create(db);
        } else {
            ok = ok && testConnection(db);
            ok = ok && upgradeIfRequired(db);
        }

        if (db.open())
            db.close();
    }

    QSqlDatabase::removeDatabase(connectionName);
    return ok;
}

bool Database::create(QSqlDatabase& db) {
    qWarning() << "Database not found. Creating...";
    return db.open() && createSchema(db) && populateInfoTable(db);
}

bool Database::createSchema(QSqlDatabase& db) {
    QSqlQuery query{db};
    return createTables(query)
            && createViews(query)
            && createTriggers(query);
}

bool Database::createTables(QSqlQuery& query) {
    const QString createInfoTable{
            "CREATE TABLE " + InfoTable::name + "("
                    + InfoTable::Columns::id + " INTEGER PRIMARY KEY, "
                    + InfoTable::Columns::name + " TEXT, "
                    + InfoTable::Columns::value + " TEXT);"
    };

    const QString createTaskTable{
            "CREATE TABLE " + TaskTable::name + "("
                    + TaskTable::Columns::id + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                    + TaskTable::Columns::name + " TEXT, "
                    + TaskTable::Columns::estimatedCost + " INTEGER, "
                    + TaskTable::Columns::actualCost + " INTEGER, "
                    + TaskTable::Columns::completed + " BOOLEAN, "
                    + TaskTable::Columns::priority + " INTEGER, "
                    + TaskTable::Columns::lastModified + " DATETIME, "
                    + TaskTable::Columns::uuid + " TEXT UNIQUE NOT NULL)"
    };

    const QString createSprintTable{
            "CREATE TABLE " + SprintTable::name + "("
                    + SprintTable::Columns::id + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                    + SprintTable::Columns::taskUuid + " TEXT, "
                    + SprintTable::Columns::startTime + " DATETIME, "
                    + SprintTable::Columns::finishTime + " DATETIME, "
                    + SprintTable::Columns::uuid + " TEXT UNIQUE NOT NULL, "
                    + "FOREIGN KEY (" + SprintTable::Columns::taskUuid + ") "
                    + "REFERENCES " + TaskTable::name + "(" + TaskTable::Columns::uuid + ")"
                    + " ON DELETE CASCADE)"
    };

    const QString createTagTable{
            "CREATE TABLE " + TagTable::name
                    + "(" + TagTable::Columns::id + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                    + TagTable::Columns::name + " TEXT UNIQUE NOT NULL)"
    };

    const QString createTaskTagTable{
            "CREATE TABLE " + TaskTagTable::name + "("
                    + TaskTagTable::Columns::id + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                    + TaskTagTable::Columns::tagId + " INTEGER NOT NULL, "
                    + TaskTagTable::Columns::taskId + " INTEGER NOT NULL, "
                    + TaskTagTable::Columns::taskUuid + " TEXT NOT NULL, "
                    + "FOREIGN KEY(" + TaskTagTable::Columns::tagId + ")"
                    + " REFERENCES " + TagTable::name + "(" + TagTable::Columns::id + ")"
                    + " ON DELETE CASCADE ON UPDATE CASCADE, "
                    + "FOREIGN KEY(" + TaskTagTable::Columns::taskId + ")"
                    + " REFERENCES " + TaskTable::name + "(" + TaskTable::Columns::id + ")"
                    + " ON DELETE CASCADE ON UPDATE CASCADE)"
    };

    return execAndCheck(query, createInfoTable)
            && execAndCheck(query, createTaskTable)
            && execAndCheck(query, createTagTable)
            && execAndCheck(query, createSprintTable)
            && execAndCheck(query, createTaskTagTable);
}

bool Database::createViews(QSqlQuery& query) {
    // This view is a workaround to let insertion of tags
    // when we don't know tag id of already existing tag
    QString createTaskTagView{
            "CREATE VIEW " + TaskTagView::name + " AS "
                    + "SELECT " + TaskTable::name + "." + TaskTable::Columns::id + ", "
                    + TaskTable::name + "." + TaskTable::Columns::name + ", "
                    + TaskTable::Columns::estimatedCost + ", "
                    + TaskTable::Columns::actualCost + ", "
                    + TaskTable::Columns::priority + ", "
                    + TaskTable::Columns::completed + ", "
                    + TagTable::name + "." + TagTable::Columns::name + " " + TaskTagView::Aliases::tagName + ", "
                    + TaskTable::Columns::lastModified + ", "
                    + TaskTable::Columns::uuid
                    + " FROM " + TaskTable::name
                    + " JOIN " + TaskTagTable::name + " ON "
                    + TaskTable::name + "." + TaskTable::Columns::id + " = " + TaskTagTable::name + "." + TaskTagTable::Columns::taskId
                    + " LEFT JOIN " + TagTable::name + " ON "
                    + TaskTagTable::name + "." + TaskTagTable::Columns::tagId + " = " + TagTable::name + "." + TagTable::Columns::id
                    + " ORDER BY " + TaskTable::name + "." + TaskTable::Columns::priority + ";"
    };

    // View for sprints
    const QString createSprintView{
            "CREATE VIEW " + SprintView::name + " AS "
                    + "SELECT " + SprintTable::name + "." + SprintTable::Columns::id + ", "
                    +  SprintTable::name + "." + SprintTable::Columns::taskUuid + ", "
                    +  TaskTable::name + "." + TaskTable::Columns::name + ", "
                    + "GROUP_CONCAT( " + TagTable::name + "." + TagTable::Columns::name + ") " + SprintView::Aliases::tags + ", "
                    + SprintTable::Columns::startTime + ", "
                    + SprintTable::Columns::finishTime + ", "
                    + SprintTable::name + "." + SprintTable::Columns::uuid
                    + " FROM " + SprintTable::name
                    + " JOIN " + TaskTable::name + " ON "
                    + SprintTable::name + "." + SprintTable::Columns::taskUuid + " = " + TaskTable::name + "." + TaskTable::Columns::uuid
                    + " LEFT JOIN " + TaskTagTable::name + " ON " + TaskTagTable::name + "." + TaskTagTable::Columns::taskId + " = " + TaskTable::name + "." + TaskTable::Columns::id
                    + " LEFT JOIN " + TagTable::name
                    + " ON " + TagTable::name + "." + TagTable::Columns::id + " = " + TaskTagTable::name + "." + TaskTagTable::Columns::tagId
                    + " GROUP BY " + SprintTable::name + "." + SprintTable::Columns::id + ";"
    };

    const QString createTaskView{
            "CREATE VIEW " + TasksView::name + " AS "
                    + "SELECT " + TaskTable::name + "." + TaskTable::Columns::id + ", "
                    + TaskTable::name + "." + TaskTable::Columns::name + ", "
                    + TaskTable::Columns::estimatedCost + ", "
                    + TaskTable::Columns::actualCost + ", "
                    + TaskTable::Columns::priority + ", "
                    + TaskTable::Columns::completed + ", "
                    + "GROUP_CONCAT(" + TagTable::name + "." + TagTable::Columns::name + ") " +  TasksView::Aliases::tags + ", "
                    + TaskTable::Columns::lastModified + ", "
                    + TaskTable::Columns::uuid
                    + " FROM " + TaskTable::name
                    + " LEFT JOIN " + TaskTagTable::name + " ON "
                    + TaskTable::name + "." + TaskTable::Columns::id + " = " + TaskTagTable::name + "." + TaskTagTable::Columns::taskId
                    + " LEFT JOIN " + TagTable::name +  " ON "
                    + TaskTagTable::name + "." + TaskTagTable::Columns::tagId + " = " + TagTable::name + "." + TagTable::Columns::id
                    + " GROUP BY " + TaskTable::name + "." + TaskTable::Columns::id
                    + " ORDER BY " + TaskTable::name + "." + TaskTable::Columns::priority + ";"
    };

    return execAndCheck(query, createTaskTagView)
            && execAndCheck(query, createSprintView)
            && execAndCheck(query, createTaskView);
}

bool Database::createTriggers(QSqlQuery& query) {
    const QString createInsteadOfTaskTagInsertTrigger{
            "CREATE TRIGGER " + TaskTagInsertTrigger::name
                    + " INSTEAD OF INSERT ON " + TaskTagView::name
                    + " BEGIN INSERT INTO " + TagTable::name + "(" + TagTable::Columns::name +")"
                    + " SELECT NEW." + TaskTagView::Aliases::tagName
                    + " WHERE (SELECT COUNT(" + TagTable::Columns::name + ")"
                    + " FROM " + TagTable::name
                    + " WHERE " + TagTable::Columns::name + " = NEW." + TaskTagView::Aliases::tagName +") = 0; "
                    + " INSERT OR IGNORE INTO "
                    + TaskTagTable::name + "(" + TaskTagTable::Columns::tagId + ", " + TaskTagTable::Columns::taskId + ", " + TaskTagTable::Columns::taskUuid + ")"
                    + " VALUES((SELECT " + TagTable::Columns::id +
                    + " FROM " + TagTable::name
                    + " WHERE " + TagTable::Columns::name + " = NEW." + TaskTagView::Aliases::tagName + "), "
                    + "(SELECT " + TaskTable::Columns::id
                    + " FROM " + TaskTable::name
                    + " WHERE " + TaskTable::Columns::uuid + " = NEW." + TaskTable::Columns::uuid + "), NEW." + TaskTable::Columns::uuid + "); "
                    + "END;"
    };

    // Trigger to remove orphaned tags (tags, that are not bound to any task)
    const QString createCleanOrphanedTagTrigger{
            "CREATE TRIGGER " + CleanOrphanedTagTrigger::name
                    + " AFTER DELETE ON " + TaskTagTable::name
                    + " BEGIN "
                    + "DELETE FROM " + TagTable::name
                    + " WHERE " + TagTable::Columns::id + " = OLD." + TaskTagTable::Columns::tagId + " AND "
                    + "(SELECT count(*) FROM " + TaskTagTable::name
                    + " WHERE " + TagTable::Columns::id + " = OLD." + TaskTagTable::Columns::tagId + ") = 0; "
                    + "END;"
    };

    // Trigger to remove from sprint as views are read-only in Sqlite3
    const QString createSprintViewDeleteTrigger{
            "CREATE TRIGGER " + SprintViewDeleteTrigger::name
                    + " INSTEAD OF DELETE ON " + SprintView::name
                    + " BEGIN "
                    + "DELETE FROM " + SprintTable::name
                    + " WHERE " + SprintTable::Columns::uuid + " = OLD." + SprintTable::Columns::uuid + "; "
                    + " END;"
    };

    const QString createSprintViewInsertTrigger{
            "CREATE TRIGGER " + SprintViewInsertTrigger::name
                    + " INSTEAD OF INSERT ON " + SprintView::name
                    + " BEGIN "
                    + "INSERT INTO " + SprintTable::name + "("
                    + SprintTable::Columns::taskUuid + ", "
                    + SprintTable::Columns::startTime + ", "
                    + SprintTable::Columns::finishTime + ", "
                    + SprintTable::Columns::uuid + ") "
                    + "SELECT"
                    + " NEW." + SprintTable::Columns::taskUuid
                    + ", NEW." + SprintTable::Columns::startTime
                    + ", NEW." + SprintTable::Columns::finishTime
                    + ", NEW." + SprintTable::Columns::uuid + "; "
                    + "END;"
    };

    // Trigger to remove from task_view
    const QString createTaskViewDeleteTrigger{
            "CREATE TRIGGER " + TaskViewDeleteTrigger::name
                    + " INSTEAD OF DELETE ON " + TasksView::name
                    + " BEGIN "
                    + "DELETE FROM " + TaskTable::name
                    + " WHERE " + TaskTable::Columns::id + " = OLD." + TaskTable::Columns::id + ";"
                    + " END;"
    };

    // Trigger on update on task_view
    const QString createTaskViewUpdateTrigger{
            "CREATE TRIGGER " + TaskViewUpdateTrigger::name
                    + " INSTEAD OF UPDATE ON " + TasksView::name
                    + " BEGIN "
                    + "UPDATE " + TaskTable::name
                    + " SET " + TaskTable::Columns::name + " = NEW." + TaskTable::Columns::name + ", "
                    + TaskTable::Columns::estimatedCost + " = NEW." + TaskTable::Columns::estimatedCost + ", "
                    + TaskTable::Columns::actualCost + " = NEW." + TaskTable::Columns::actualCost + ", "
                    + TaskTable::Columns::priority + " = NEW." + TaskTable::Columns::priority + ", "
                    + TaskTable::Columns::completed + " = NEW." + TaskTable::Columns::completed + ", "
                    + TaskTable::Columns::lastModified + " = NEW." + TaskTable::Columns::lastModified
                    + " WHERE " + TaskTable::Columns::id + " = OLD." + TaskTable::Columns::id + ";"
                    + " END;"
    };

    return execAndCheck(query, createInsteadOfTaskTagInsertTrigger)
            && execAndCheck(query, createCleanOrphanedTagTrigger)
            && execAndCheck(query, createSprintViewDeleteTrigger)
            && execAndCheck(query, createSprintViewInsertTrigger)
            && execAndCheck(query, createTaskViewDeleteTrigger)
            && execAndCheck(query, createTaskViewUpdateTrigger);
}

bool Database::populateInfoTable(QSqlDatabase& database) const
{
    // Populate info table with current database version
    QSqlQuery query{database};
    const QString queryStr = QString{"insert into %1 (%2, %3) values(:name, :value)"}
        .arg(InfoTable::name, InfoTable::Columns::name, InfoTable::Columns::value);
    query.prepare(queryStr);
    query.bindValue(":name", "version");
    query.bindValue(":value", QString{"%1"}.arg(currentDatabaseVersion));
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::testConnection(QSqlDatabase& database)
{
    qWarning() << "Testing database connection...";
    if (!database.open()) {
        QString message = QString{"Cannot open database connection\n Error: %1"}
                .arg(database.lastError().text());
        qCritical() << message;
        return false;
    }
    qWarning() << "test connection established";
    return true;
}

bool Database::upgradeIfRequired(QSqlDatabase& database) {
    auto version = getDatabaseVersion(database);

    qDebug() << "Latest database version is " << currentDatabaseVersion;
    qDebug() << "Actual database version is " << version;

    if (version < currentDatabaseVersion) {
        qDebug() << "Running migration...";
        if (!createBackupCopy(database.databaseName()))
            qCritical() << "Reserve copy creation failed, migration aborted";

        if (!runMigration(database, version)) {
            qCritical() << "Migration failed, restoring database from reserve copy...";
            if (restoreBackupCopy(database.databaseName()))
                qCritical() << "Database restored";

            return false;
        }

        qDebug() << "Migration successful";

        updateVersion(database);
    }

    return true;
}

unsigned Database::getDatabaseVersion(QSqlDatabase& database)
{
    QSqlQuery query{database};

    const QString getVersionQuery = QString{"select %1 from %2 where %3 = (:name)"}
            .arg(InfoTable::Columns::value)
            .arg(InfoTable::name)
            .arg(InfoTable::Columns::name);
    query.prepare(getVersionQuery);
    query.bindValue(":name", "version");

    if (!query.exec()) {
        return false;
    }

    if (!query.first())
        return false;

    QString versionStr{query.value(0).toString()};
    auto version = versionStr.toInt();
    query.finish();

    return version;
}

bool Database::runMigration(QSqlDatabase& database, int fromVersion)
{
    QSqlQuery query{database};

    query.exec("PRAGMA FOREIGN_KEYS = OFF");

    if (fromVersion == 1) {
        qDebug() << "Running migration Version"
                 << fromVersion
                 << " -> Version"
                 << currentDatabaseVersion;

        database.transaction();

        auto execRollbackOnFail = [this, &query, &database](const QString& queryStr) {
            if (!execAndCheck(query, queryStr)) {
                database.rollback();
                return false;
            }
            return true;
        };

        bool ok = execRollbackOnFail("DROP INDEX todo_item_uuid_uindex;")
                && execRollbackOnFail(
                        "CREATE TABLE task ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "name TEXT, "
                                "estimated_cost INTEGER, "
                                "actual_cost INTEGER, "
                                "completed BOOLEAN, "
                                "priority INTEGER, "
                                "last_modified TEXT, "
                                "uuid TEXT);")
                && execRollbackOnFail(
                        "CREATE UNIQUE INDEX task_uuid_uindex ON task (uuid);")
                && execRollbackOnFail(
                        "INSERT INTO task(id, name, estimated_cost, actual_cost, completed, "
                                "priority, last_modified, uuid) "
                                "SELECT id, name, estimated_pomodoros, spent_pomodoros, completed, "
                                " priority, last_modified, uuid from todo_item;")
                && execRollbackOnFail("DROP TABLE todo_item;")
                && execRollbackOnFail(
                        "CREATE TABLE sprint ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "start_time DATETIME, "
                                "finish_time DATETIME, "
                                "uuid TEXT NOT NULL, "
                                "task_uuid TEXT, "
                                "FOREIGN KEY (task_uuid) REFERENCES task(uuid) "
                                "ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED);")
                && execRollbackOnFail(
                        "INSERT INTO sprint(id, start_time, finish_time, uuid, task_uuid) "
                                "SELECT id, start_time, finish_time, uuid, todo_uuid "
                                "FROM pomodoro;")
                && execRollbackOnFail("DROP TABLE pomodoro;")
                && execRollbackOnFail(
                        "CREATE TABLE tasktag("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "tag_id INTEGER NOT NULL, "
                                "task_id INTEGER NOT NULL, "
                                "task_uuid TEXT, "
                                "FOREIGN KEY (task_id) REFERENCES task(id) ON DELETE CASCADE ON UPDATE CASCADE "
                                "DEFERRABLE INITIALLY DEFERRED, "
                                "FOREIGN KEY (tag_id) REFERENCES tag(id) ON DELETE CASCADE ON UPDATE CASCADE "
                                "DEFERRABLE INITIALLY DEFERRED);")
                && execRollbackOnFail(
                        "INSERT INTO tasktag(id, tag_id, task_id, task_uuid) "
                                "SELECT id, tag_id, todo_id, todo_uuid FROM todotag;")
                && execRollbackOnFail("DROP TABLE todotag")
                && execRollbackOnFail("DRop view task_tag_view")
                && execRollbackOnFail("drop view task_view")
                && execRollbackOnFail("drop view pomodoro_view")
                && createViews(query)
                && createTriggers(query);

        query.exec("PRAGMA FOREIGN_KEYS = OFF");

        query.finish();

        if (ok) {
            return database.commit();
        }
    }

    if (fromVersion == 2) {
        return query.exec("drop table calendar;");
    }

    return false;
}

bool Database::createBackupCopy(const QString& filename)
{
    if (QFile::exists(filename + ".old"))
        QFile::remove(filename + ".old");
    return QFile::copy(filename, filename + ".old");
}

bool Database::restoreBackupCopy(const QString& filename)
{
    if (QFile::exists(filename))
        QFile::remove(filename);
    return QFile::copy(filename + ".old", filename);
}

bool Database::updateVersion(QSqlDatabase& database) const {
    QSqlQuery query{database};
    const QString queryStr = QString{
            "update %1 set %2 = (:value) where %3 = (:name)"}
    .arg(InfoTable::name)
    .arg(InfoTable::Columns::value)
    .arg(InfoTable::Columns::name);
    query.prepare(queryStr);
    query.bindValue(":name", "version");
    query.bindValue(":value", QString{"%1"}.arg(currentDatabaseVersion));
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::execAndCheck(QSqlQuery& query, const QString& queryStr) {
    bool ok = query.exec(queryStr);
    if (!ok) {
        qDebug() << "Error when running";
        qDebug() << queryStr;
        qDebug() << query.lastError();
    }
    return ok;
}

