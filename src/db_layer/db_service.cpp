#include "db_service.h"
// #include "utils/UuidUtils.h"
#include <exception>


DBService::DBService(QString filename)
{
    Worker* worker = new Worker(filename);
    worker->moveToThread(&workerThread);
    qRegisterMetaType<std::vector<QSqlRecord>>("std::vector<QSqlRecord>");
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &DBService::queue, worker, &Worker::execute);
    connect(this, &DBService::queuePrepared, worker, &Worker::executePrepared);
    connect(worker, &Worker::results, this, &DBService::handleResults);
    connect(worker, &Worker::error, this, &DBService::handleError);
    connect(this, &DBService::prepareQuery, worker, &Worker::prepare);
    connect(this, &DBService::bind, worker, &Worker::bindValue);
    // connect(this,
    //         &DBService::executeTransaction,
    //         worker,
    //         &Worker::executeTransaction);
    workerThread.start();
}


DBService::~DBService()
{
    workerThread.quit();
    workerThread.wait();
}


long long DBService::executeQuery(const QString& query)
{
    emit queue(nextQueryId, query);
    return nextQueryId++;
}


long long DBService::prepare(const QString& query)
{
    emit prepareQuery(nextQueryId, query);
    return nextQueryId++;
}


void DBService::executePrepared(long long queryId)
{
    emit queuePrepared(queryId);
}


// void DBService::executeInTransaction(const std::vector<long long> ids)
// {
//     emit executeTransaction(ids);
// }


void DBService::handleResults(long long queryId,
                              const std::vector<QSqlRecord>& records)
{
    emit results(queryId, records);
}


void DBService::handleError(long long queryId, const QString& errorMessage)
{
    qDebug() << queryId;
    qDebug() << errorMessage;
    emit error(queryId, errorMessage);
}


void DBService::bindValue(long long queryId,
                          const QString& placeholder,
                          const QVariant& value)
{
    emit bind(queryId, placeholder, value);
}


Worker::Worker(const QString& filename)
    : filename{std::move(filename)}
    , db{QSqlDatabase::addDatabase("QSQLITE")}
{
    if (!createDbConnection())
        throw std::runtime_error("Unable to create database");
}


void Worker::execute(long long queryId, const QString& query)
{
    QSqlQuery q;
    if (!q.exec(query)) {
        QString errormsg
            = QString("%1 %2").arg(q.lastQuery()).arg(q.lastError().text());
        emit error(queryId, errormsg);
    }
    else {
        std::vector<QSqlRecord> recs;
        while (q.next()) {
            recs.push_back(q.record());
        }
        emit results(queryId, recs);
    }
}

void Worker::executePrepared(long long queryId)
{
    // TODO handle missing id case
    QSqlQuery query = preparedQueries.value(queryId);
    if (!query.exec()) {
        QString errormsg = QString("%1 %2")
                               .arg(query.lastQuery())
                               .arg(query.lastError().text());
        emit error(queryId, errormsg);
    }
    else {
        std::vector<QSqlRecord> recs;
        while (query.next()) {
            recs.push_back(query.record());
        }
        emit results(queryId, recs);
    }
}


// void executeTransaction(const std::vector<long long> ids)
// {
//     QSqlDatabase::database().transaction();
//     for (const auto& queryId : ids) {
//         // TODO handle missing id case
//         QSqlQuery query = preparedQueries.value(queryId);
//         if (!query.exec()) {
//             QString errormsg = QString("Error executing transaction");
//             emit error(queryId, errormsg);
//             QSqlDatabase::database().rollback();
//             return;
//         }
//     }
//     QSqlDatabase::database().commit();
// }


void Worker::prepare(long long queryId, const QString& queryStr)
{
    QSqlQuery query;
    query.prepare(queryStr);
    preparedQueries.insert(queryId, query);
}


void Worker::bindValue(long long queryId,
                       const QString& placeholder,
                       const QVariant& value)
{
    // QSqlQuery* query = preparedQueries.value(queryId).get();
    preparedQueries[queryId].bindValue(placeholder, value);
    // query->bindValue(placeholder, value);
}


Worker::~Worker()
{
    db.close();
    QSqlDatabase::removeDatabase("QSQLITE");
}

bool Worker::createDatabase()
{
    db.setDatabaseName(filename);
    return db.open() && createSchema();
}


bool Worker::createDbConnection()
{
    // QString filename = "db/pomodoro.db";
    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QFile databaseFile(filename);
    if (filename == ":memory:") {
        db.open();
        createSchema();
        activateForeignKeys();
        return true;
    }
    // if (filename == ":memory:" || !databaseFile.exists()) {
    if (!databaseFile.exists()) {
        qDebug() << "Database not found. Creating...";
        if (!createDatabase()) {
            return false;
        }
    }
    else {
        qDebug() << "Database found. Opening...";
        db.setDatabaseName(filename);
    }

    if (!db.open()) {
        qDebug() << "Failed to open database";
        return false;
    }
    activateForeignKeys();
    // migrate();
    return true;
}

// TODO remove
void Worker::migrate()
{
    BoostUUIDGenerator generator;
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString qstr{"select id from pomodoro;"};
    query.exec(qstr);
    while (query.next()) {
        long long id = query.value(0).toUInt();
        QSqlQuery q;
        QString uuid = QString::fromStdString(generator.generateUUID());
        q.prepare("update pomodoro set uuid = :uuid where id = :id");
        q.bindValue(":uuid", QVariant(uuid));
        q.bindValue(":id", QVariant(id));
        q.exec();
    }
    QSqlDatabase::database().commit();
    query.exec("select id from todo_item;");
    QSqlDatabase::database().transaction();
    while (query.next()) {
        long long id = query.value(0).toUInt();
        QSqlQuery m;
        QString uuid = QString::fromStdString(generator.generateUUID());
        m.prepare("update todo_item set uuid = :uuid where id = :id");
        m.bindValue(":uuid", QVariant(uuid));
        m.bindValue(":id", QVariant(id));
        m.exec();
    }
    QSqlDatabase::database().commit();
}


bool Worker::createSchema()
{
    QSqlQuery query;

    QString createTodoItemTable = "CREATE TABLE todo_item "
                                  "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "name VARCHAR, "
                                  "estimated_pomodoros INTEGER, "
                                  "spent_pomodoros INTEGER, "
                                  "completed BOOLEAN, "
                                  "priority INTEGER, "
                                  "last_modified DATETIME, "
                                  "uuid TEXT UNIQUE NOT NULL)";

    QString createPomodoroTable
        = "CREATE TABLE pomodoro "
          "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "todo_id INTEGER, "
          "start_time DATETIME, "
          "finish_time DATETIME, "
          "uuid TEXT UNIQUE NOT NULL, "
          "todo_uuid TEXT, "
          "FOREIGN KEY (todo_uuid) REFERENCES todo_item(uuid) "
          "ON DELETE CASCADE)";

    QString createTagTable = "CREATE TABLE tag "
                             "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "name VARCHAR(15) UNIQUE NOT NULL)";

    QString createTodoTagTable = "CREATE TABLE todotag "
                                 "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "tag_id INTEGER NOT NULL, "
                                 "todo_id INTEGER NOT NULL, "
                                 "FOREIGN KEY(tag_id) REFERENCES tag(id), "
                                 "FOREIGN KEY(todo_id) REFERENCES "
                                 "todo_item(id) ON DELETE CASCADE)";

    // Trigger to remove orphaned tags (tags, that are not bound to any todo
    // item)
    QString createCleanOrphanedTagTrigger
        = "CREATE TRIGGER on_todotag_delete "
          "AFTER DELETE ON todotag "
          "BEGIN "
          "DELETE FROM tag WHERE id = old.tag_id AND "
          "(SELECT count(*) FROM todotag "
          "WHERE tag_id = old.tag_id) = 0;"
          "END;";

    // Trigger to increment spent pomodoros in todo_item when new pomodoro
    // is
    // inserted
    // QString createIncrementSpentTrigger
    //     = "CREATE TRIGGER increment_spent_after_pomo_insert "
    //       "AFTER INSERT ON pomodoro "
    //       "BEGIN "
    //       "UPDATE todo_item SET spent_pomodoros = spent_pomodoros + 1 "
    //       "WHERE todo_item.uuid = NEW.todo_uuid; "
    //       "END;";

    // Trigger to decrement spent pomodoros in todo_item after pomodoro is
    // removed
    // QString createDecrementSpentTrigger
    //     = "CREATE TRIGGER decrement_spent_after_pomo_delete "
    //       "AFTER DELETE ON pomodoro "
    //       "BEGIN "
    //       "UPDATE todo_item SET spent_pomodoros = spent_pomodoros - 1 "
    //       "WHERE todo_item.uuid = OLD.todo_uuid; "
    //       "END;";

    // View for pomodoros
    QString createPomodoroView
        = "CREATE VIEW pomodoro_view AS "
          "SELECT pomodoro.id, pomodoro.todo_uuid, todo_item.name, "
          "group_concat(tag.name) tags, start_time, finish_time, "
          "pomodoro.uuid "
          "FROM pomodoro join todo_item ON pomodoro.todo_uuid = "
          "todo_item.uuid "
          "LEFT JOIN todotag ON todotag.todo_id = todo_item.id "
          "LEFT JOIN tag ON tag.id = todotag.tag_id "
          "GROUP BY pomodoro.id;";

    // Trigger to remove from pomodoro_view as view are read-only in Sqlite3
    QString createPomodoroViewDeleteTrigger
        = "CREATE TRIGGER delete_from_pomodoro_view "
          "INSTEAD OF DELETE ON pomodoro_view "
          "BEGIN "
          "DELETE FROM pomodoro WHERE uuid = old.uuid; "
          "END;";

    QString createPomodoroViewInsertTrigger
        = "CREATE TRIGGER instead_pomodoro_view_insert "
          "INSTEAD OF INSERT "
          "ON pomodoro_view "
          "BEGIN "
          "INSERT INTO pomodoro (todo_uuid, start_time, finish_time, uuid) "
          "SELECT new.todo_uuid, new.start_time, new.finish_time, new.uuid;"
          "END;";

    // View for todo items
    QString createTaskView
        = "CREATE VIEW task_view AS "
          "SELECT todo_item.id, "
          "todo_item.name, "
          "estimated_pomodoros, "
          "spent_pomodoros, "
          "priority, "
          "completed, "
          "GROUP_CONCAT(tag.name) tags, "
          "last_modified, "
          "uuid "
          "FROM todo_item "
          "LEFT JOIN todotag ON todo_item.id = todotag.todo_id "
          "LEFT JOIN tag ON todotag.tag_id = tag.id "
          "GROUP BY todo_item.id "
          "ORDER BY todo_item.priority;";

    // Trigger to remove from task_view
    QString createTaskViewDeleteTrigger
        = "CREATE TRIGGER on_task_view_delete "
          "INSTEAD OF DELETE ON task_view "
          "BEGIN "
          "DELETE FROM todo_item WHERE id = old.id; "
          "END;";

    // Trigger on update on task_view
    QString createTaskViewUpdateTrigger
        = "CREATE TRIGGER on_task_view_update "
          "INSTEAD OF UPDATE ON task_view "
          "BEGIN "
          "UPDATE todo_item SET name = new.name, "
          "estimated_pomodoros = new.estimated_pomodoros, "
          "spent_pomodoros = new.spent_pomodoros, "
          "priority = new.priority, "
          "completed = new.completed, "
          "last_modified = new.last_modified "
          "WHERE id = old.id; "
          "END;";

    // Create calendar table
    query.exec("CREATE TABLE calendar (id INTEGER PRIMARY KEY)");
    query.exec("INSERT INTO calendar DEFAULT VALUES");
    query.exec("INSERT INTO calendar DEFAULT VALUES");
    query.exec("INSERT INTO calendar SELECT NULL FROM calendar d1, calendar "
               "d2, calendar d3, calendar d4");
    query.exec("INSERT INTO calendar SELECT NULL FROM calendar d1, calendar "
               "d2, calendar d3, calendar d4");
    query.exec("ALTER TABLE calendar ADD dt DATETIME");
    query.exec(
        "UPDATE calendar SET dt = date('2000-01-01', (-1 + id) ||' day')");
    query.exec("CREATE INDEX idx1 ON calendar(dt)");

    return execAndCheck(query, createTodoItemTable)
        && execAndCheck(query, createTagTable)
        && execAndCheck(query, createPomodoroTable)
        && execAndCheck(query, createTodoTagTable)
        && execAndCheck(query, createCleanOrphanedTagTrigger)
        // && execAndCheck(query, createIncrementSpentTrigger)
        // && execAndCheck(query, createDecrementSpentTrigger)
        && execAndCheck(query, createPomodoroView)
        && execAndCheck(query, createPomodoroViewDeleteTrigger)
        && execAndCheck(query, createPomodoroViewInsertTrigger)
        && execAndCheck(query, createTaskView)
        && execAndCheck(query, createTaskViewDeleteTrigger)
        && execAndCheck(query, createTaskViewUpdateTrigger);
}


bool Worker::activateForeignKeys()
{
    QSqlQuery query;
    return query.exec("PRAGMA foreign_keys = ON");
}


bool Worker::execAndCheck(QSqlQuery& query, const QString& queryStr)
{
    bool ok = query.exec(queryStr);
    if (!ok) {
        qDebug() << "Error when running";
        qDebug() << queryStr;
        qDebug() << query.lastError();
    }
    return ok;
}
