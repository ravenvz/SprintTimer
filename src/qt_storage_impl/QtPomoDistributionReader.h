#ifndef QTSQLITEPOMODORODAILYDISTRIBUTIONREADER_H_TS4GUJR3
#define QTSQLITEPOMODORODAILYDISTRIBUTIONREADER_H_TS4GUJR3

#include "core/IPomodoroDistributionReader.h"
#include "db_layer/db_service.h"
#include <QObject>


class DistributionReaderBase : public QObject,
                               public IPomodoroDistributionReader {

    Q_OBJECT

public:
    DistributionReaderBase(DBService& dbService);

    virtual void requestDailyDistribution(const TimeSpan& timeSpan,
                                          Handler handler) final;

private slots:

    virtual void onResultsReceived(long long queryId,
                                   const std::vector<QSqlRecord>& records);

protected:
    DBService& dbService;
    Handler handler;
    long long mQueryId;
};


class QtPomoDailyDistributionReader : public DistributionReaderBase {
public:
    QtPomoDailyDistributionReader(DBService& dbService);
};

class QtPomoWeeklyDistributionReader : public DistributionReaderBase {
public:
    QtPomoWeeklyDistributionReader(DBService& dbService);
};

class QtPomoMonthlyDistributionReader
    : public DistributionReaderBase {
public:
    QtPomoMonthlyDistributionReader(DBService& dbService);
};


#endif /* end of include guard:                                                \
          QTSQLITEPOMODORODAILYDISTRIBUTIONREADER_H_TS4GUJR3 */
