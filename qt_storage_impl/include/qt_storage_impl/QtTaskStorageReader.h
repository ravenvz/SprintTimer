/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#ifndef QTTASKSTORAGEREADER_H_48BDKQZ7
#define QTTASKSTORAGEREADER_H_48BDKQZ7

#include "core/ITaskStorageReader.h"
#include "qt_storage_impl/DBService.h"
#include <QObject>

class QtTaskStorageReader : public QObject, public ITaskStorageReader {
    Q_OBJECT

public:
    explicit QtTaskStorageReader(DBService& dbService);

    void requestUnfinishedTasks(Handler handler) final;

    void requestFinishedTasks(const TimeSpan& timeSpan, Handler handler) final;

    void requestTasks(const TimeSpan& timeSpan, Handler handler) final;

    void requestAllTags(TagHandler handler) final;

private:
    enum class Column {
        Id,
        Name,
        EstimatedCost,
        ActualCost,
        Priority,
        Completed,
        Tags,
        LastModified,
        Uuid
    };

    enum class TagColumn { Id, Name };

    qint64 mUnfinishedQueryId{-1};
    qint64 mFinishedQueryId{-1};
    qint64 mRequestTasksQueryId{-1};
    qint64 mTagQueryId{-1};
    DBService& dbService;
    std::list<Handler> handler_queue;
    std::list<TagHandler> tag_handler_queue;

    QVariant columnData(const QSqlRecord& record, Column column) const;

    Task taskFromQSqlRecord(const QSqlRecord& record) const;

    std::string tagFromSqlRecord(const QSqlRecord& record) const;

private slots:
    void onResultsReceived(qint64 queryId,
                           const std::vector<QSqlRecord>& records);
};

#endif /* end of include guard: QTTASKSTORAGEREADER_H_48BDKQZ7 */
