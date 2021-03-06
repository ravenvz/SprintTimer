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
#ifndef QTSPRINTSTORAGEREADER_H_JXULCJ6I
#define QTSPRINTSTORAGEREADER_H_JXULCJ6I

#include "core/ISprintStorageReader.h"
#include "core/entities/Tag.h"
#include "qt_storage_impl/DBService.h"
#include <QObject>
#include <queue>

namespace sprint_timer::storage::qt_storage_impl {

class QtSprintStorageReader : public QObject, public ISprintStorageReader {
    Q_OBJECT

public:
    explicit QtSprintStorageReader(DBService& dbService);

    void requestItems(const dw::DateRange& dateRange, Handler handler) final;

    void sprintsForTask(const std::string& taskUuid, Handler handler) final;

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
    qint64 sprintsInTimeRangeQueryId{-1};
    qint64 sprintsForTaskQueryId{-1};
    DBService& dbService;
    std::queue<Handler> handlerQueue;

    entities::Sprint sprintFromQSqlRecord(const QSqlRecord& record);

    QVariant columnData(const QSqlRecord& record, Columns column);

    bool listeningToQueryId(qint64 queryId) const;

private slots:
    void onResultsReceived(qint64 queryId,
                           const std::vector<QSqlRecord>& records);
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: QTSPRINTSTORAGEREADER_H_JXULCJ6I */
