#ifndef QTSQLITEPOMODOROYEARRANGEREADER_H_6LK9HHEM
#define QTSQLITEPOMODOROYEARRANGEREADER_H_6LK9HHEM

#include "core/IPomodoroYearRangeReader.h"
#include "db_layer/db_service.h"
#include <QObject>
#include <functional>

class QtPomoYearRangeReader : public QObject, public IPomodoroYearRangeReader {
    Q_OBJECT

public:
    QtPomoYearRangeReader(DBService& dbService);

    void requestYearRange(Handler handler) final;

private:
    DBService& dbService;
    Handler handler;
    long long mQueryId{-1};

private slots:
    void onResultsReceived(long long queryId,
                           const std::vector<QSqlRecord>& records);
};


#endif /* end of include guard: QTSQLITEPOMODOROYEARRANGEREADER_H_6LK9HHEM */
