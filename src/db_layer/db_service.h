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

public slots:

    void executeQuery(const QString& query);

    void executePrepared(const QString& queryId);

    void handleResults(const std::vector<QSqlRecord>& records);

    void handleError(const QString& errorMessage);

    void bindValue(const QString& query,
                   const QString& placeholder,
                   const QVariant& value);

signals:
    void queue(const QString& query);
    void queuePrepared(const QString& queryId);
    void results(const std::vector<QSqlRecord>& records);
    void prepare(const QString& queryId, const QString& queryStr);
    void bind(const QString& query,
              const QString& placeholder,
              const QVariant& value);

private:
    QThread workerThread;
};


class Worker : public QObject {
    Q_OBJECT

public:
    Worker(const QString& filename);
    ~Worker();

public slots:
    void execute(const QString& query);

    void executePrepared(const QString& queryId);

    void prepare(const QString& queryId, const QString& queryStr);

    void bindValue(const QString& queryId,
                   const QString& placeholder,
                   const QVariant& value);

signals:
    void results(const std::vector<QSqlRecord>& records);

    void error(const QString& errorMessage);

private:
    QString filename;
    QSqlDatabase db;
    // QHash<QString, std::unique_ptr<QSqlQuery>> preparedQueries;
    QHash<QString, QSqlQuery> preparedQueries;

    bool createDatabase();

    bool createSchema();

    bool createDbConnection();

    bool activateForeignKeys();

    bool execAndCheck(QSqlQuery& query, const QString& queryStr);
};


// bool createDatabase(QSqlDatabase& db, QString& filename);
//
// bool createSchema();
//
// bool createDbConnection();
//
// bool activateForeignKeys();
//

#endif // DB_HELPER_H
