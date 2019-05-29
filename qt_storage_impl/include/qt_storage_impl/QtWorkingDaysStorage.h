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
#ifndef QTWORKINGDAYSSTORAGE_H_EXLIBP9M
#define QTWORKINGDAYSSTORAGE_H_EXLIBP9M

#include "qt_storage_impl/DBService.h"
#include <QObject>
#include <core/IWorkingDaysStorage.h>
#include <queue>

namespace sprint_timer::storage::qt_storage_impl {

class QtWorkingDaysStorage : public QObject, public IWorkingDaysStorage {
public:
    QtWorkingDaysStorage(DBService& dbService, QObject* parent = nullptr);

    void changeWorkingDays(const WorkdayTracker& tracker) override;

    void requestData(IWorkingDaysReader::ResultHandler resultHandler) override;

private:
    struct Context {
        IWorkingDaysReader::ResultHandler handler;
        WorkdayTracker tracker;
    };

    DBService& dbService;
    qint64 storeSchedulesQueryId{-1};
    qint64 storeExceptionalDaysQuery{-1};
    QString requestSchedulesQuery;
    QString requestExceptionalDaysQuery;
    qint64 requestSchedulesQueryId{-1};
    qint64 requestExceptionalDaysQueryId{-1};
    std::queue<Context> contexts;

    void removeExtraDays(const std::vector<dw::Date>& days);

    void onResultReceived(qint64 queryId,
                          const std::vector<QSqlRecord>& records);
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: QTWORKINGDAYSSTORAGE_H_EXLIBP9M */
