#ifndef QTSQLITEPOMODORODAILYDISTRIBUTIONREADER_H_TS4GUJR3
#define QTSQLITEPOMODORODAILYDISTRIBUTIONREADER_H_TS4GUJR3

#include "core/IPomodoroDailyDistributionReader.h"
#include "db_layer/db_service.h"
#include <QObject>

class QtSqlitePomodoroDailyDistributionReader
    : public QObject,
      public IPomodoroDailyDistributionReader {

    Q_OBJECT

public:
    QtSqlitePomodoroDailyDistributionReader(DBService& dbService);

    void requestDailyDistribution(const TimeSpan& timeSpan,
                                  Handler handler) final;

private slots:

    void onResultsReceived(const QString& queryId,
                           const std::vector<QSqlRecord>& records);

private:
    DBService& dbService;
    QString mQueryId{"RequestDailyDistribution"};
    Handler handler;
};

#endif /* end of include guard:                                                \
          QTSQLITEPOMODORODAILYDISTRIBUTIONREADER_H_TS4GUJR3 */
