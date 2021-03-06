/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef QTWORKINGDAYSREADER_H_0RFFMJYS
#define QTWORKINGDAYSREADER_H_0RFFMJYS

#include "qt_storage_impl/DBService.h"
#include <core/IConfig.h>
#include <core/IWorkingDaysReader.h>
#include <queue>

namespace sprint_timer::storage::qt_storage_impl {

class QtWorkingDaysReader : public QObject, public IWorkingDaysReader {
    Q_OBJECT

public:
    QtWorkingDaysReader(DBService& dbService, const IConfig& settings);

    void requestData(IWorkingDaysReader::ResultHandler resultHandler) override;

private:
    struct Context {
        IWorkingDaysReader::ResultHandler handler;
        WorkdayTracker tracker;
    };

    DBService& dbService;
    const IConfig& settings;
    QString requestSchedulesQuery;
    QString requestExceptionalDaysQuery;
    qint64 requestSchedulesQueryId{-1};
    qint64 requestExceptionalDaysQueryId{-1};
    std::queue<Context> contexts;

    void onResultReceived(qint64 queryId,
                          const std::vector<QSqlRecord>& records);
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: QTWORKINGDAYSREADER_H_0RFFMJYS */

