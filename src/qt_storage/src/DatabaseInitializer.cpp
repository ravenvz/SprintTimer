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
#include "qt_storage/DatabaseInitializer.h"
#include "qt_storage/ConnectionGuard.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/DatabaseError.h"
#include "qt_storage/QueryError.h"
#include "qt_storage/migrations/MigrationManager.h"
#include "qt_storage/migrations/Migration_v3.h"
#include "qt_storage/migrations/Migration_v4.h"
#include "qt_storage/migrations/Migration_v5.h"
#include "qt_storage/migrations/Migration_v6.h"
#include "qt_storage/migrations/Migration_v7.h"
#include "qt_storage/utils/QueryUtils.h"
#include <QDebug>
#include <QSqlDriver>
#include <QSqlError>
#include <QStringBuilder>
#include <QtCore/QFile>

namespace {

using namespace sprint_timer::storage::qt_storage;

constexpr unsigned currentDatabaseVersion{7};

bool databaseFileNotFound(const QString& filePath);

void create(QSqlDatabase& db);

void createSchema(QSqlDatabase& db);

void populateInfoTable(QSqlDatabase& database);

void createTables(QSqlQuery& query);

void createViews(QSqlQuery& query);

void createTriggers(QSqlQuery& query);

MigrationManager prepareMigrationManager(QSqlDatabase& database);

} // namespace

namespace sprint_timer::storage::qt_storage {

DatabaseInitializer::DatabaseInitializer(const QString& filename)
{
    const QString connectionName{"SprintTimerDesktop"};
    const bool dbIsNew = databaseFileNotFound(filename);

    ConnectionGuard connectionGuard{filename, connectionName};

    auto db = QSqlDatabase::database(connectionName);

    if (dbIsNew) {
        create(db);
    }

    const auto migrationManager = prepareMigrationManager(db);
    migrationManager.runMigrations(connectionName);
}

} // namespace sprint_timer::storage::qt_storage

namespace {

using namespace sprint_timer::storage::qt_storage;

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
        "CREATE TABLE " % InfoTable::name % "(" % InfoTable::Columns::id %
        " INTEGER PRIMARY KEY, " % InfoTable::Columns::name % " TEXT, " %
        InfoTable::Columns::value % " TEXT);"};

    const QString createTaskTable{
        "CREATE TABLE " % TaskTable::name % "(" % TaskTable::Columns::id %
        " INTEGER PRIMARY KEY AUTOINCREMENT, " % TaskTable::Columns::uuid %
        " TEXT UNIQUE NOT NULL, " % TaskTable::Columns::name % " TEXT, " %
        TaskTable::Columns::estimatedCost % " INTEGER, " %
        TaskTable::Columns::completed % " BOOLEAN, " %
        TaskTable::Columns::lastModified % " DATETIME, " %
        TaskTable::Columns::deleted % " BOOLEAN DEFAULT 0);"};

    const QString createSprintTable{
        "CREATE TABLE " % SprintTable::name % "(" % SprintTable::Columns::id %
        " INTEGER PRIMARY KEY AUTOINCREMENT, " % SprintTable::Columns::task_id %
        " INTEGER, " % SprintTable::Columns::startTime % " DATETIME, " %
        SprintTable::Columns::finishTime % " DATETIME, " %
        SprintTable::Columns::deleted % " BOOLEAN DEFAULT 0, " %
        "FOREIGN KEY (" % SprintTable::Columns::task_id % ") " % "REFERENCES " %
        TaskTable::name % "(" % TaskTable::Columns::id %
        ") ON DELETE CASCADE);"};

    const QString createTagTable{
        "CREATE TABLE " % TagTable::name % "(" % TagTable::Columns::id %
        " INTEGER PRIMARY KEY AUTOINCREMENT, " % TagTable::Columns::name %
        " TEXT UNIQUE NOT NULL)"};

    const QString createTaskTagTable{
        "CREATE TABLE " % TaskTagTable::name % "(" % TaskTagTable::Columns::id %
        " INTEGER PRIMARY KEY AUTOINCREMENT, " % TaskTagTable::Columns::tagId %
        " INTEGER NOT NULL, " % TaskTagTable::Columns::taskId %
        " INTEGER NOT NULL, " % TaskTagTable::Columns::taskUuid %
        " TEXT NOT NULL, " % "FOREIGN KEY(" % TaskTagTable::Columns::tagId %
        ")" % " REFERENCES " % TagTable::name % "(" % TagTable::Columns::id %
        ")" % " ON DELETE CASCADE ON UPDATE CASCADE, " % "FOREIGN KEY(" %
        TaskTagTable::Columns::taskId % ")" % " REFERENCES " % TaskTable::name %
        "(" % TaskTable::Columns::id % ")" %
        " ON DELETE CASCADE ON UPDATE CASCADE)"};

    const QString createExceptionalDayTable{
        "CREATE TABLE " % ExceptionalDayTable::name % "(" %
        ExceptionalDayTable::Columns::id %
        " INTEGER PRIMARY KEY AUTOINCREMENT, " %
        ExceptionalDayTable::Columns::date % " DATE UNIQUE, " %
        ExceptionalDayTable::Columns::goal % " INTEGER);"};

    const QString createScheduleTable{
        "CREATE TABLE " % ScheduleTable::name % "(" %
        ScheduleTable::Columns::id % " INTEGER PRIMARY KEY AUTOINCREMENT, " %
        ScheduleTable::Columns::applied_since % " DATE UNIQUE, " %
        ScheduleTable::Columns::monday_goal % " INTEGER, " %
        ScheduleTable::Columns::tuesday_goal % " INTEGER, " %
        ScheduleTable::Columns::wednesday_goal % " INTEGER, " %
        ScheduleTable::Columns::thursday_goal % " INTEGER, " %
        ScheduleTable::Columns::friday_goal % " INTEGER, " %
        ScheduleTable::Columns::saturday_goal % " INTEGER, " %
        ScheduleTable::Columns::sunday_goal % " INTEGER);"};

    const QString createTaskTimeframeTable{
        "CREATE TABLE IF NOT EXISTS " % TaskTimeframeTable::name % "(" %
        TaskTimeframeTable::Columns::id %
        " INTEGER PRIMARY KEY AUTOINCREMENT, " %
        TaskTimeframeTable::Columns::task_id % " INTEGER UNIQUE NOT NULL, " %
        TaskTimeframeTable::Columns::start % " DATETIME DEFAULT NULL, " %
        TaskTimeframeTable::Columns::due % " DATETIME DEFAULT NULL, " %
        TaskTimeframeTable::Columns::reminder % " DATETIME DEFAULT NULL, " %
        TaskTimeframeTable::Columns::recurrence % " TEXT DEFAULT NULL, " %
        "FOREIGN KEY (" % TaskTimeframeTable::Columns::task_id %
        ") REFERENCES " % TaskTable::name % "(" % TaskTable::Columns::id %
        ") ON DELETE CASCADE);"};

    const QString createNotesTable{
        "CREATE TABLE IF NOT EXISTS " % NotesTable::name % " (" %
        NotesTable::Columns::id % " INTEGER PRIMARY KEY AUTOINCREMENT, " %
        NotesTable::Columns::task_id % " INTEGER UNIQUE NOT NULL, " %
        NotesTable::Columns::text % " STRING, FOREIGN KEY (" %
        NotesTable::Columns::task_id % ") REFERENCES " % TaskTable::name % "(" %
        TaskTable::Columns::id % ") ON DELETE CASCADE);"};

    tryExecute(query, createInfoTable);
    tryExecute(query, createTaskTable);
    tryExecute(query, createTagTable);
    tryExecute(query, createSprintTable);
    tryExecute(query, createTaskTagTable);
    tryExecute(query, createExceptionalDayTable);
    tryExecute(query, createScheduleTable);
    tryExecute(query, createNotesTable);
    tryExecute(query, createTaskTimeframeTable);
}

void createViews(QSqlQuery& query)
{
    const QString createTaskTagView{
        "CREATE VIEW " % TaskTagView::name % " AS " % "SELECT " %
        TaskTable::name % "." % TaskTable::Columns::id % ", " %
        TaskTable::name % "." % TaskTable::Columns::name % " " %
        TaskTagView::Aliases::name % ", " % TaskTable::Columns::estimatedCost %
        ", " % TaskTable::Columns::completed % ", " % TagTable::name % "." %
        TagTable::Columns::name % " " % TaskTagView::Aliases::tagName % ", " %
        TaskTable::Columns::lastModified % ", " % TaskTable::Columns::uuid %
        " FROM " % TaskTable::name % " JOIN " % TaskTagTable::name % " ON " %
        TaskTable::name % "." % TaskTable::Columns::id % " = " %
        TaskTagTable::name % "." % TaskTagTable::Columns::taskId %
        " LEFT JOIN " % TagTable::name % " ON " % TaskTagTable::name % "." %
        TaskTagTable::Columns::tagId % " = " % TagTable::name % "." %
        TagTable::Columns::id % " WHERE " % TaskTable::Columns::deleted %
        " = 0;"};

    const QString createSprintView{
        "CREATE VIEW " % SprintView::name % " AS " % "SELECT " %
        TaskTable::name % "." % TaskTable::Columns::id % " " %
        SprintView::Aliases::taskid % ", " % TaskTable::name % "." %
        TaskTable::Columns::name % ", " % "GROUP_CONCAT(" % TagTable::name %
        "." % TagTable::Columns::name % ") " % SprintView::Aliases::tags %
        ", " % SprintTable::Columns::startTime % ", " %
        SprintTable::Columns::finishTime % " FROM " % SprintTable::name %
        " JOIN " % TaskTable::name % " ON " % SprintTable::name % "." %
        SprintTable::Columns::task_id % " = " % TaskTable::name % "." %
        TaskTable::Columns::id % " LEFT JOIN " % TaskTagTable::name % " ON " %
        TaskTagTable::name % "." % TaskTagTable::Columns::taskId % " = " %
        TaskTable::name % "." % TaskTable::Columns::id % " LEFT JOIN " %
        TagTable::name % " ON " % TagTable::name % "." % TagTable::Columns::id %
        " = " % TaskTagTable::name % "." % TaskTagTable::Columns::tagId %
        " WHERE " % SprintTable::name % "." % SprintTable::Columns::deleted %
        " = 0 " % " GROUP BY " % SprintTable::name % "." %
        SprintTable::Columns::id % ";"};

    const QString createCleanSprintView{
        "CREATE VIEW " % CleanSprintView::name % " AS SELECT " %
        SprintTable::Columns::id % ", " % SprintTable::Columns::task_id % ", " %
        SprintTable::Columns::startTime % ", " %
        SprintTable::Columns::finishTime % " FROM " % " " % SprintTable::name %
        " WHERE " % SprintTable::Columns::deleted % " = 0;"};

    const QString createTaskView{
        "CREATE VIEW " % TasksView::name % " AS " % "SELECT " %
        TaskTable::name % "." % TaskTable::Columns::id % " " %
        TasksView::Aliases::task_id % ", " % TaskTable::name % "." %
        TaskTable::Columns::name % " " % TasksView::Aliases::name % ", " %
        TaskTable::Columns::estimatedCost % ", " %
        TaskTable::Columns::completed % ", " % "GROUP_CONCAT(" %
        TagTable::name % "." % TagTable::Columns::name % ") " %
        TasksView::Aliases::tags % ", " % TaskTable::Columns::lastModified %
        ", " % TaskTable::Columns::uuid % ", " % NotesTable::Columns::text %
        ", " % TaskTimeframeTable::Columns::start % ", " %
        TaskTimeframeTable::Columns::due % ", " %
        TaskTimeframeTable::Columns::reminder % ", " %
        TaskTimeframeTable::Columns::recurrence % " FROM " % TaskTable::name %
        " LEFT JOIN " % TaskTagTable::name % " ON " % TaskTable::name % "." %
        TaskTable::Columns::id % " = " % TaskTagTable::name % "." %
        TaskTagTable::Columns::taskId % " LEFT JOIN " % TagTable::name %
        " ON " % TaskTagTable::name % "." % TaskTagTable::Columns::tagId %
        " = " % TagTable::name % "." % TagTable::Columns::id % " LEFT JOIN " %
        NotesTable::name % " ON " % TaskTable::name % "." %
        TaskTable::Columns::id % " = " % NotesTable::name % "." %
        NotesTable::Columns::task_id % " LEFT JOIN " %
        TaskTimeframeTable::name % " ON " % TaskTable::name % "." %
        TaskTable::Columns::id % " = " % TaskTimeframeTable::name % "." %
        TaskTimeframeTable::Columns::task_id % " WHERE " %
        TaskTable::Columns::deleted % " = 0 GROUP BY " % TaskTable::name % "." %
        TaskTable::Columns::id};

    const QString createAdvTaskView{
        "CREATE VIEW " % AdvTaskView::name % " AS " % "SELECT " %
        TaskTable::Columns::uuid % ", " % TaskTable::Columns::name % ", " %
        TaskTable::Columns::estimatedCost % ", " %
        TaskTable::Columns::completed % ", " % TasksView::Aliases::tags % ", " %
        TaskTable::Columns::lastModified % ", " %
        SprintTable::Columns::startTime % ", " %
        SprintTable::Columns::finishTime % ", " % NotesTable::Columns::text %
        ", " % TaskTimeframeTable::Columns::start % ", " %
        TaskTimeframeTable::Columns::due % ", " %
        TaskTimeframeTable::Columns::reminder % ", " %
        TaskTimeframeTable::Columns::recurrence % " FROM " % TasksView::name %
        " LEFT JOIN " % CleanSprintView::name % " ON " % CleanSprintView::name %
        "." % SprintTable::Columns::task_id % " = " % TasksView::name % "." %
        TasksView::Aliases::task_id % ";"};

    tryExecute(query, createTaskTagView);
    tryExecute(query, createSprintView);
    tryExecute(query, createCleanSprintView);
    tryExecute(query, createTaskView);
    tryExecute(query, createAdvTaskView);
}

void createTriggers(QSqlQuery& query)
{
    const QString createInsteadOfTaskTagInsertTrigger{
        "CREATE TRIGGER " % TaskTagInsertTrigger::name %
        " INSTEAD OF INSERT ON " % TaskTagView::name % " BEGIN INSERT INTO " %
        TagTable::name % "(" % TagTable::Columns::name % ")" % " SELECT NEW." %
        TaskTagView::Aliases::tagName % " WHERE (SELECT COUNT(" %
        TagTable::Columns::name % ")" % " FROM " % TagTable::name % " WHERE " %
        TagTable::Columns::name % " = NEW." % TaskTagView::Aliases::tagName %
        ") = 0; " % " INSERT OR IGNORE INTO " % TaskTagTable::name % "(" %
        TaskTagTable::Columns::tagId % ", " % TaskTagTable::Columns::taskId %
        ", " % TaskTagTable::Columns::taskUuid % ")" % " VALUES((SELECT " %
        TagTable::Columns::id % " FROM " % TagTable::name % " WHERE " %
        TagTable::Columns::name % " = NEW." % TaskTagView::Aliases::tagName %
        "), " % "(SELECT " % TaskTable::Columns::id % " FROM " %
        TaskTable::name % " WHERE " % TaskTable::Columns::uuid % " = NEW." %
        TaskTable::Columns::uuid % "), NEW." % TaskTable::Columns::uuid %
        "); " % "END;"};

    // Trigger to remove orphaned tags (tags, that are not bound to any task)
    const QString createCleanOrphanedTagTrigger{
        "CREATE TRIGGER " % CleanOrphanedTagTrigger::name %
        " AFTER DELETE ON " % TaskTagTable::name % " BEGIN " % "DELETE FROM " %
        TagTable::name % " WHERE " % TagTable::Columns::id % " = OLD." %
        TaskTagTable::Columns::tagId % " AND " % "(SELECT count(*) FROM " %
        TaskTagTable::name % " WHERE " % TaskTagTable::Columns::tagId %
        " = OLD." % TaskTagTable::Columns::tagId % ") = 0; " % "END;"};

    // Trigger to remove from sprint as views are read-only in Sqlite3
    const QString createSprintViewDeleteTrigger{
        "CREATE TRIGGER " % SprintViewDeleteTrigger::name %
        " INSTEAD OF DELETE ON " % SprintView::name % " BEGIN " %
        "DELETE FROM " % SprintTable::name % " WHERE " %
        SprintTable::Columns::startTime % " = OLD." %
        SprintTable::Columns::startTime % "; END;"};

    const QString createSprintViewInsertTrigger{
        "CREATE TRIGGER " % SprintViewInsertTrigger::name %
        " INSTEAD OF INSERT ON " % SprintView::name % " BEGIN " %
        "INSERT INTO " % SprintTable::name % "(" %
        SprintTable::Columns::task_id % ", " % SprintTable::Columns::startTime %
        ", " % SprintTable::Columns::finishTime % ") SELECT NEW." %
        SprintView::Aliases::taskid % ", NEW." %
        SprintTable::Columns::startTime % ", NEW." %
        SprintTable::Columns::finishTime % "; END;"};

    // Trigger to remove from task_view
    const QString createTaskViewDeleteTrigger{
        "CREATE TRIGGER " % TaskViewDeleteTrigger::name %
        " INSTEAD OF DELETE ON " % TasksView::name % " BEGIN " %
        "DELETE FROM " % TaskTable::name % " WHERE " % TaskTable::Columns::id %
        " = OLD." % TaskTable::Columns::id % ";" % " END;"};

    // Trigger on update on task_view
    const QString createTaskViewUpdateTrigger{
        "CREATE TRIGGER " % TaskViewUpdateTrigger::name %
        " INSTEAD OF UPDATE ON " % TasksView::name % " BEGIN " % "UPDATE " %
        TaskTable::name % " SET " % TaskTable::Columns::name % " = NEW." %
        TaskTable::Columns::name % ", " % TaskTable::Columns::estimatedCost %
        " = NEW." % TaskTable::Columns::estimatedCost % ", " %
        TaskTable::Columns::completed % " = NEW." %
        TaskTable::Columns::completed % ", " % TaskTable::Columns::deleted %
        " = OLD." % TaskTable::Columns::deleted % ", " %
        TaskTable::Columns::lastModified % " = NEW." %
        TaskTable::Columns::lastModified % " WHERE " % TaskTable::Columns::id %
        " = OLD." % TaskTable::Columns::id % ";" % " END;"};

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
        "INSERT INTO " % InfoTable::name % " (" % InfoTable::Columns::name %
        "," % InfoTable::Columns::value % ") VALUES(:name, :value)"};
    QSqlQuery query{database};
    query.prepare(populateInfoTableQuery);
    query.bindValue(":name", "version");
    query.bindValue(":value", QString{"%1"}.arg(currentDatabaseVersion));
    if (!query.exec()) {
        throw QueryError{"Error updating database version", query};
    }
}

MigrationManager prepareMigrationManager(QSqlDatabase& /*database*/)
{
    MigrationManager migrationManager{currentDatabaseVersion};
    migrationManager.addMigration(2, std::make_unique<Migration_v3>());
    migrationManager.addMigration(3, std::make_unique<Migration_v4>());
    migrationManager.addMigration(4, std::make_unique<Migration_v5>());
    migrationManager.addMigration(5, std::make_unique<Migration_v6>());
    migrationManager.addMigration(6, std::make_unique<Migration_v7>());
    return migrationManager;
}

} // namespace
