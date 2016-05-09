#ifndef QTSQLITE_H_BVPQF5OG
#define QTSQLITE_H_BVPQF5OG

#include "core/IPomodoroStorageReader.h"
#include "core/use_cases/use_cases.h"
#include "db_service.h"
#include <QDebug>
#include <QObject>

class QtSqlitePomodoroYearRangeReader
    : public QObject,
      public UseCases::IPomodoroYearRangeReader {
    Q_OBJECT

public:
    using Handler = std::function<void(const std::vector<std::string>&)>;

    QtSqlitePomodoroYearRangeReader(DBService& dbService, Handler handler);

    void requestYearRange() final;

private:
    DBService& dbService;
    Handler handler;

private slots:
    void onResultsReceived(const std::vector<QSqlRecord>& records);
};


class QtSqlitePomodoroStorageReader : public QObject,
                                      public IPomodoroStorageReader {
    Q_OBJECT

public:
    QtSqlitePomodoroStorageReader(DBService& dbService);

    void requestItems(const TimeSpan& timeSpan, Handler handler) final;

private:
    enum class Columns {
        Id = 0,
        TodoId,
        Name,
        Tags,
        StartTime,
        FinishTime,
    };
    const QString queryId{"pomodoroStorageReader"};
    DBService& dbService;
    Handler handler;

    Pomodoro pomodoroFromQSqlRecord(const QSqlRecord& record);
    QVariant columnData(const QSqlRecord& record, Columns column);

private slots:
    void onResultsReceived(const std::vector<QSqlRecord>& records);
};

#endif /* end of include guard: QTSQLITE_H_BVPQF5OG */
