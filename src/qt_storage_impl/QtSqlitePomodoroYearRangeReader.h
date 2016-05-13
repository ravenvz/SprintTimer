#ifndef QTSQLITEPOMODOROYEARRANGEREADER_H_6LK9HHEM
#define QTSQLITEPOMODOROYEARRANGEREADER_H_6LK9HHEM

#include "core/IPomodoroYearRangeReader.h"
#include <QObject>
#include <functional>
#include "db_layer/db_service.h"

class QtSqlitePomodoroYearRangeReader : public QObject,
                                        public IPomodoroYearRangeReader {
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


#endif /* end of include guard: QTSQLITEPOMODOROYEARRANGEREADER_H_6LK9HHEM */
