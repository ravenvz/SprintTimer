/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef QTSPRINTDISTRIBUTIONREADER_H_TS4GUJR3
#define QTSPRINTDISTRIBUTIONREADER_H_TS4GUJR3

#include "core/ISprintDistributionReader.h"
#include "qt_storage_impl/db_service.h"
#include <QObject>


class DistributionReaderBase : public QObject,
                               public ISprintDistributionReader {

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
    std::list<Handler> handler_queue;
    long long mQueryId{-1};
};


class QtSprintDailyDistributionReader : public DistributionReaderBase {
public:
    QtSprintDailyDistributionReader(DBService& dbService);
};

class QtSprintWeeklyDistributionReader : public DistributionReaderBase {
public:
    QtSprintWeeklyDistributionReader(DBService& dbService);
};

class QtSprintMonthlyDistributionReader : public DistributionReaderBase {
public:
    QtSprintMonthlyDistributionReader(DBService& dbService);
};

#endif /* end of include guard: QTSPRINTDISTRIBUTIONREADER_H_TS4GUJR3 */
