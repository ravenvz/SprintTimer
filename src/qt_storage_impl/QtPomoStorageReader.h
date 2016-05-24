#ifndef QTSQLITEPOMODOROSTORAGEREADER_H_JXULCJ6I
#define QTSQLITEPOMODOROSTORAGEREADER_H_JXULCJ6I

#include "core/IPomodoroStorageReader.h"
#include "db_layer/db_service.h"
#include <QObject>


class QtPomoStorageReader : public QObject, public IPomodoroStorageReader {
    Q_OBJECT

public:
    explicit QtPomoStorageReader(DBService& dbService);

    void requestItems(const TimeSpan& timeSpan, Handler handler) final;

private:
    enum class Columns {
        Id = 0,
        TodoUuid,
        Name,
        Tags,
        StartTime,
        FinishTime,
        Uuid,
    };
    long long mQueryId{-1};
    DBService& dbService;
    Handler handler;

    Pomodoro pomodoroFromQSqlRecord(const QSqlRecord& record);

    QVariant columnData(const QSqlRecord& record, Columns column);

private slots:
    void onResultsReceived(long long queryId,
                           const std::vector<QSqlRecord>& records);
};


#endif /* end of include guard: QTSQLITEPOMODOROSTORAGEREADER_H_JXULCJ6I */
