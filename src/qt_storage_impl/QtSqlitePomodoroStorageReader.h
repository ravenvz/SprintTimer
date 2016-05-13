#ifndef QTSQLITEPOMODOROSTORAGEREADER_H_JXULCJ6I
#define QTSQLITEPOMODOROSTORAGEREADER_H_JXULCJ6I

#include <QObject>
#include "core/IPomodoroStorageReader.h"
#include "db_layer/db_service.h"


class QtSqlitePomodoroStorageReader : public QObject,
                                      public IPomodoroStorageReader {
    Q_OBJECT

public:
    explicit QtSqlitePomodoroStorageReader(DBService& dbService);

    void requestItems(const TimeSpan& timeSpan, Handler handler) final;

private:
    enum class Columns {
        Id = 0,
        TodoId,
        Name,
        Tags,
        StartTime,
        FinishTime,
        Uuid,
    };
    const QString queryId{"pomodoroStorageReader"};
    DBService& dbService;
    Handler handler;

    Pomodoro pomodoroFromQSqlRecord(const QSqlRecord& record);

    QVariant columnData(const QSqlRecord& record, Columns column);

private slots:
    void onResultsReceived(const std::vector<QSqlRecord>& records);
    void onError(const QString& errorMessage);
};



#endif /* end of include guard: QTSQLITEPOMODOROSTORAGEREADER_H_JXULCJ6I */
