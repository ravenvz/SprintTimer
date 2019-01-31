/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#include "qt_storage_impl/DatabaseInitializer.h"
#include "qt_storage_impl/ConnectionGuard.h"
#include "qt_storage_impl/DatabaseDescription.h"
#include "qt_storage_impl/DatabaseError.h"
#include "qt_storage_impl/QueryError.h"
#include "qt_storage_impl/migrations/MigrationManager.h"
#include "qt_storage_impl/migrations/Migration_v3.h"
#include "qt_storage_impl/migrations/Migration_v4.h"
#include "qt_storage_impl/migrations/Migration_v5.h"
#include "qt_storage_impl/utils/QueryUtils.h"
#include <QDebug>
#include <QSqlDriver>
#include <QSqlError>
#include <QStringBuilder>
#include <QtCore/QFile>

namespace {

using namespace sprint_timer::storage::qt_storage_impl;

constexpr unsigned currentDatabaseVersion{5};

bool databaseFileNotFound(const QString& filePath);

void create(QSqlDatabase& db);

void createSchema(QSqlDatabase& db);

void populateInfoTable(QSqlDatabase& database);

void createTables(QSqlQuery& query);

void createViews(QSqlQuery& query);

void createTriggers(QSqlQuery& query);

MigrationManager prepareMigrationManager(QSqlDatabase& database);

} // namespace

namespace sprint_timer::storage::qt_storage_impl {


DatabaseInitializer::DatabaseInitializer(const QString& filename)
{
    const QString connectionName{"SprintTimerDesktop"};
    const bool dbIsNew = databaseFileNotFound(filename);

    ConnectionGuard connectionGuard{filename, connectionName};

    auto db = QSqlDatabase::database(connectionName);

    if (dbIsNew)
        create(db);

    const auto migrationManager = prepareMigrationManager(db);
    migrationManager.runMigrations(db);
}

} // namespace sprint_timer::storage::qt_storage_impl

namespace {

using namespace sprint_timer::storage::qt_storage_impl;


bool databaseFileNotFound(const QString& filePath)
{
	if (filePath == ":memory:" || filePath == "file::memory:?cache=shared")
		return true;
    return !QFile::exists(filePath);
}

void create(QSqlDatabase& db)
{
    qWarning() << "Database not found. Creating...";
    createSchema(db);
    populateInfoTable(db);
}

void createSchema(QSqlDatabase& db)
{
    QSqlQuery query{db};
    createTables(query);
    createViews(query);
    createTriggers(query);
}

void createTables(QSqlQuery& query)
{
    const QString createInfoTable{
        "CREATE TABLE " % InfoTable::name % "(" % InfoTable::Columns::id
        % " INTEGER PRIMARY KEY, " % InfoTable::Columns::name % " TEXT, "
        % InfoTable::Columns::value % " TEXT);"};

    const QString createTaskTable{
        "CREATE TABLE " % TaskTable::name % "(" % TaskTable::Columns::id
        % " INTEGER PRIMARY KEY AUTOINCREMENT, " % TaskTable::Columns::name
        % " TEXT, " % TaskTable::Columns::estimatedCost % " INTEGER, "
        % TaskTable::Columns::actualCost % " INTEGER, "
        % TaskTable::Columns::completed % " BOOLEAN, "
        % TaskTable::Columns::priority % " INTEGER, "
        % TaskTable::Columns::lastModified % " DATETIME, "
        % TaskTable::Columns::uuid % " TEXT UNIQUE NOT NULL)"};

    const QString createSprintTable{
        "CREATE TABLE " % SprintTable::name % "(" % SprintTable::Columns::id
        % " INTEGER PRIMARY KEY AUTOINCREMENT, "
        % SprintTable::Columns::taskUuid % " TEXT, "
        % SprintTable::Columns::startTime % " DATETIME, "
        % SprintTable::Columns::finishTime % " DATETIME, "
        % SprintTable::Columns::uuid % " TEXT UNIQUE NOT NULL, "
        % "FOREIGN KEY (" % SprintTable::Columns::taskUuid % ") "
        % "REFERENCES " % TaskTable::name % "(" % TaskTable::Columns::uuid % ")"
        % " ON DELETE CASCADE)"};

    const QString createTagTable{
        "CREATE TABLE " % TagTable::name % "(" % TagTable::Columns::id
        % " INTEGER PRIMARY KEY AUTOINCREMENT, " % TagTable::Columns::name
        % " TEXT UNIQUE NOT NULL)"};

    const QString createTaskTagTable{
        "CREATE TABLE " % TaskTagTable::name % "(" % TaskTagTable::Columns::id
        % " INTEGER PRIMARY KEY AUTOINCREMENT, " % TaskTagTable::Columns::tagId
        % " INTEGER NOT NULL, " % TaskTagTable::Columns::taskId
        % " INTEGER NOT NULL, " % TaskTagTable::Columns::taskUuid
        % " TEXT NOT NULL, " % "FOREIGN KEY(" % TaskTagTable::Columns::tagId
        % ")" % " REFERENCES " % TagTable::name % "(" % TagTable::Columns::id
        % ")" % " ON DELETE CASCADE ON UPDATE CASCADE, " % "FOREIGN KEY("
        % TaskTagTable::Columns::taskId % ")" % " REFERENCES " % TaskTable::name
        % "(" % TaskTable::Columns::id % ")"
        % " ON DELETE CASCADE ON UPDATE CASCADE)"};

    tryExecute(query, createInfoTable);
    tryExecute(query, createTaskTable);
    tryExecute(query, createTagTable);
    tryExecute(query, createSprintTable);
    tryExecute(query, createTaskTagTable);
}

void createViews(QSqlQuery& query)
{
    QString createTaskTagView{
        "CREATE VIEW " % TaskTagView::name % " AS " % "SELECT "
        % TaskTable::name % "." % TaskTable::Columns::id % ", "
        % TaskTable::name % "." % TaskTable::Columns::name % ", "
        % TaskTable::Columns::estimatedCost % ", "
        % TaskTable::Columns::actualCost % ", " % TaskTable::Columns::priority
        % ", " % TaskTable::Columns::completed % ", " % TagTable::name % "."
        % TagTable::Columns::name % " " % TaskTagView::Aliases::tagName % ", "
        % TaskTable::Columns::lastModified % ", " % TaskTable::Columns::uuid
        % " FROM " % TaskTable::name % " JOIN " % TaskTagTable::name % " ON "
        % TaskTable::name % "." % TaskTable::Columns::id % " = "
        % TaskTagTable::name % "." % TaskTagTable::Columns::taskId
        % " LEFT JOIN " % TagTable::name % " ON " % TaskTagTable::name % "."
        % TaskTagTable::Columns::tagId % " = " % TagTable::name % "."
        % TagTable::Columns::id % " ORDER BY " % TaskTable::name % "."
        % TaskTable::Columns::priority % ";"};

    const QString createSprintView{
        "CREATE VIEW " % SprintView::name % " AS " % "SELECT "
        % SprintTable::name % "." % SprintTable::Columns::id % ", "
        % SprintTable::name % "." % SprintTable::Columns::taskUuid % ", "
        % TaskTable::name % "." % TaskTable::Columns::name % ", "
        % "GROUP_CONCAT( " % TagTable::name % "." % TagTable::Columns::name
        % ") " % SprintView::Aliases::tags % ", "
        % SprintTable::Columns::startTime % ", "
        % SprintTable::Columns::finishTime % ", " % SprintTable::name % "."
        % SprintTable::Columns::uuid % " FROM " % SprintTable::name % " JOIN "
        % TaskTable::name % " ON " % SprintTable::name % "."
        % SprintTable::Columns::taskUuid % " = " % TaskTable::name % "."
        % TaskTable::Columns::uuid % " LEFT JOIN " % TaskTagTable::name % " ON "
        % TaskTagTable::name % "." % TaskTagTable::Columns::taskId % " = "
        % TaskTable::name % "." % TaskTable::Columns::id % " LEFT JOIN "
        % TagTable::name % " ON " % TagTable::name % "." % TagTable::Columns::id
        % " = " % TaskTagTable::name % "." % TaskTagTable::Columns::tagId
        % " GROUP BY " % SprintTable::name % "." % SprintTable::Columns::id
        % ";"};

    const QString createTaskView{
        "CREATE VIEW " % TasksView::name % " AS " % "SELECT " % TaskTable::name
        % "." % TaskTable::Columns::id % ", " % TaskTable::name % "."
        % TaskTable::Columns::name % ", " % TaskTable::Columns::estimatedCost
        % ", " % TaskTable::Columns::actualCost % ", "
        % TaskTable::Columns::priority % ", " % TaskTable::Columns::completed
        % ", " % "GROUP_CONCAT(" % TagTable::name % "."
        % TagTable::Columns::name % ") " % TasksView::Aliases::tags % ", "
        % TaskTable::Columns::lastModified % ", " % TaskTable::Columns::uuid
        % " FROM " % TaskTable::name % " LEFT JOIN " % TaskTagTable::name
        % " ON " % TaskTable::name % "." % TaskTable::Columns::id % " = "
        % TaskTagTable::name % "." % TaskTagTable::Columns::taskId
        % " LEFT JOIN " % TagTable::name % " ON " % TaskTagTable::name % "."
        % TaskTagTable::Columns::tagId % " = " % TagTable::name % "."
        % TagTable::Columns::id % " GROUP BY " % TaskTable::name % "."
        % TaskTable::Columns::id % " ORDER BY " % TaskTable::name % "."
        % TaskTable::Columns::priority % ";"};

    tryExecute(query, createTaskTagView);
    tryExecute(query, createSprintView);
    tryExecute(query, createTaskView);
}

void createTriggers(QSqlQuery& query)
{
    const QString createInsteadOfTaskTagInsertTrigger{
        "CREATE TRIGGER " % TaskTagInsertTrigger::name
        % " INSTEAD OF INSERT ON " % TaskTagView::name % " BEGIN INSERT INTO "
        % TagTable::name % "(" % TagTable::Columns::name % ")" % " SELECT NEW."
        % TaskTagView::Aliases::tagName % " WHERE (SELECT COUNT("
        % TagTable::Columns::name % ")" % " FROM " % TagTable::name % " WHERE "
        % TagTable::Columns::name % " = NEW." % TaskTagView::Aliases::tagName
        % ") = 0; " % " INSERT OR IGNORE INTO " % TaskTagTable::name % "("
        % TaskTagTable::Columns::tagId % ", " % TaskTagTable::Columns::taskId
        % ", " % TaskTagTable::Columns::taskUuid % ")" % " VALUES((SELECT "
        % TagTable::Columns::id % " FROM " % TagTable::name % " WHERE "
        % TagTable::Columns::name % " = NEW." % TaskTagView::Aliases::tagName
        % "), " % "(SELECT " % TaskTable::Columns::id % " FROM "
        % TaskTable::name % " WHERE " % TaskTable::Columns::uuid % " = NEW."
        % TaskTable::Columns::uuid % "), NEW." % TaskTable::Columns::uuid
        % "); " % "END;"};

    // Trigger to remove orphaned tags (tags, that are not bound to any task)
    const QString createCleanOrphanedTagTrigger{
        "CREATE TRIGGER " % CleanOrphanedTagTrigger::name % " AFTER DELETE ON "
        % TaskTagTable::name % " BEGIN " % "DELETE FROM " % TagTable::name
        % " WHERE " % TagTable::Columns::id % " = OLD."
        % TaskTagTable::Columns::tagId % " AND " % "(SELECT count(*) FROM "
        % TaskTagTable::name % " WHERE " % TaskTagTable::Columns::tagId
        % " = OLD." % TaskTagTable::Columns::tagId % ") = 0; " % "END;"};

    // Trigger to remove from sprint as views are read-only in Sqlite3
    // Also increments Task's num completed sprints
    const QString createSprintViewDeleteTrigger{
        "CREATE TRIGGER " % SprintViewDeleteTrigger::name
        % " INSTEAD OF DELETE ON " % SprintView::name % " BEGIN "
        % "DELETE FROM " % SprintTable::name % " WHERE "
        % SprintTable::Columns::uuid % " = OLD." % SprintTable::Columns::uuid
        % "; " % "UPDATE " % TaskTable::name % " SET "
        % TaskTable::Columns::actualCost % " = "
        % TaskTable::Columns::actualCost % " - 1" % " WHERE "
        % TaskTable::Columns::uuid % " = OLD." % SprintTable::Columns::taskUuid
        % "; " % " END;"};

    // Also decrements Task's num completed sprints
    const QString createSprintViewInsertTrigger{
        "CREATE TRIGGER " % SprintViewInsertTrigger::name
        % " INSTEAD OF INSERT ON " % SprintView::name % " BEGIN "
        % "INSERT INTO " % SprintTable::name % "("
        % SprintTable::Columns::taskUuid % ", "
        % SprintTable::Columns::startTime % ", "
        % SprintTable::Columns::finishTime % ", " % SprintTable::Columns::uuid
        % ") " % "SELECT" % " NEW." % SprintTable::Columns::taskUuid % ", NEW."
        % SprintTable::Columns::startTime % ", NEW."
        % SprintTable::Columns::finishTime % ", NEW."
        % SprintTable::Columns::uuid % "; " % "UPDATE " % TaskTable::name
        % " SET " % TaskTable::Columns::actualCost % " = "
        % TaskTable::Columns::actualCost % " + 1" % " WHERE "
        % TaskTable::Columns::uuid % " = NEW." % SprintTable::Columns::taskUuid
        % "; " % "END;"};

    // Trigger to remove from task_view
    const QString createTaskViewDeleteTrigger{
        "CREATE TRIGGER " % TaskViewDeleteTrigger::name
        % " INSTEAD OF DELETE ON " % TasksView::name % " BEGIN "
        % "DELETE FROM " % TaskTable::name % " WHERE " % TaskTable::Columns::id
        % " = OLD." % TaskTable::Columns::id % ";" % " END;"};

    // Trigger on update on task_view
    const QString createTaskViewUpdateTrigger{
        "CREATE TRIGGER " % TaskViewUpdateTrigger::name
        % " INSTEAD OF UPDATE ON " % TasksView::name % " BEGIN " % "UPDATE "
        % TaskTable::name % " SET " % TaskTable::Columns::name % " = NEW."
        % TaskTable::Columns::name % ", " % TaskTable::Columns::estimatedCost
        % " = NEW." % TaskTable::Columns::estimatedCost % ", "
        % TaskTable::Columns::actualCost % " = NEW."
        % TaskTable::Columns::actualCost % ", " % TaskTable::Columns::priority
        % " = NEW." % TaskTable::Columns::priority % ", "
        % TaskTable::Columns::completed % " = NEW."
        % TaskTable::Columns::completed % ", "
        % TaskTable::Columns::lastModified % " = NEW."
        % TaskTable::Columns::lastModified % " WHERE " % TaskTable::Columns::id
        % " = OLD." % TaskTable::Columns::id % ";" % " END;"};

    tryExecute(query, createInsteadOfTaskTagInsertTrigger);
    tryExecute(query, createCleanOrphanedTagTrigger);
    tryExecute(query, createSprintViewDeleteTrigger);
    tryExecute(query, createSprintViewInsertTrigger);
    tryExecute(query, createTaskViewDeleteTrigger);
    tryExecute(query, createTaskViewUpdateTrigger);
}

void populateInfoTable(QSqlDatabase& database)
{
    // Populate info table with current database version
    const QString populateInfoTableQuery{
        "INSERT INTO " % InfoTable::name % " (" % InfoTable::Columns::name % ","
        % InfoTable::Columns::value % ") VALUES(:name, :value)"};
    QSqlQuery query{database};
    query.prepare(populateInfoTableQuery);
    query.bindValue(":name", "version");
    query.bindValue(":value", QString{"%1"}.arg(currentDatabaseVersion));
    if (!query.exec()) {
        throw QueryError{"Error updating database version", query};
    }
}

MigrationManager prepareMigrationManager(QSqlDatabase& database)
{
    MigrationManager migrationManager{currentDatabaseVersion};
    migrationManager.addMigration(2, std::make_unique<Migration_v3>());
    migrationManager.addMigration(3, std::make_unique<Migration_v4>());
    migrationManager.addMigration(4, std::make_unique<Migration_v5>());
    return migrationManager;
}

} // namespace
