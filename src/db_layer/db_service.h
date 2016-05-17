#ifndef DB_HELPER_H
#define DB_HELPER_H

#include "core/entities/Pomodoro.h"
#include "core/entities/TodoItem.h"
#include <QDebug>
#include <QHash>
#include <QObject>
#include <QSqlDatabase>
#include <QThread>
#include <QtSql>
#include <algorithm>
#include <memory>
#include <vector>


class Worker;

class DBService : public QObject {
    Q_OBJECT

public:
    DBService(QString filename);
    ~DBService();

    DBService(DBService&&) = default;
    DBService& operator=(DBService&&) = default;

    DBService(const DBService&) = default;
    DBService& operator=(const DBService&) = default;

    /* Execute single query and return it's id. */
    long long executeQuery(const QString& query);

    /* Prepare query and return it's id. */
    long long prepare(const QString& query);

    // TODO what if no query with such id
    /* Execute previously prepared query */
    void executePrepared(long long queryId);

    // TODO what if no query with such id
    /* Bind value to a placeholder for a previously prepared query
     * with given id. */
    void bindValue(long long queryId,
                   const QString& placeholder,
                   const QVariant& value);

public slots:

    void handleResults(long long queryId,
                       const std::vector<QSqlRecord>& records);

    void handleError(long long queryId, const QString& errorMessage);

signals:
    void queue(long long queryId, const QString& query);
    void queuePrepared(long long queryId);
    void results(long long queryId, const std::vector<QSqlRecord>& records);
    void error(long long queryId, const QString& errorMessage);
    void prepareQuery(long long queryId, const QString& queryStr);
    void
    bind(long long queryId, const QString& placeholder, const QVariant& value);

private:
    QThread workerThread;
    long long nextQueryId{0};
};


class Worker : public QObject {
    Q_OBJECT

public:
    Worker(const QString& filename);
    ~Worker();

public slots:
    void execute(long long queryId, const QString& query);

    void executePrepared(long long queryId);

    void prepare(long long queryId, const QString& queryStr);

    void bindValue(long long queryId,
                   const QString& placeholder,
                   const QVariant& value);

signals:
    void results(long long queryId, const std::vector<QSqlRecord>& records);

    void error(long long queryId, const QString& errorMessage);

private:
    QString filename;
    QSqlDatabase db;
    QHash<long long, QSqlQuery> preparedQueries;

    bool createDatabase();

    bool createSchema();

    bool createDbConnection();

    bool activateForeignKeys();

    bool execAndCheck(QSqlQuery& query, const QString& queryStr);

    void migrate();
};


#endif // DB_HELPER_H
