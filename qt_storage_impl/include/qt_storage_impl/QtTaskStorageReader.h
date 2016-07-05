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
#ifndef QTTASKSTORAGEREADER_H_48BDKQZ7
#define QTTASKSTORAGEREADER_H_48BDKQZ7

#include "core/ITaskStorageReader.h"
#include "qt_storage_impl/db_service.h"
#include <QObject>

class QtTaskStorageReader : public QObject, public ITaskStorageReader {
    Q_OBJECT

public:
    explicit QtTaskStorageReader(DBService& dbService);

    void requestUnfinishedTasks(Handler handler) final;

    void requestFinishedTasks(const TimeSpan& timeSpan, Handler handler) final;

    void requestAllTags(TagHandler handler) final;

private:
    enum class Column {
        Id,
        Name,
        EstimatedPomodoros,
        SpentPomodoros,
        Priority,
        Completed,
        Tags,
        LastModified,
        Uuid
    };

    enum class TagColumn { Id, Name };

    long long mUnfinishedQueryId{-1};
    long long mFinishedQueryId{-1};
    long long mTagQueryId{-1};
    DBService& dbService;
    std::list<Handler> handler_queue;
    std::list<TagHandler> tag_handler_queue;

    QVariant columnData(const QSqlRecord& record, Column column) const;

    Task taskFromQSqlRecord(const QSqlRecord& record) const;

    std::string tagFromSqlRecord(const QSqlRecord& record) const;

private slots:
    void onResultsReceived(long long queryId,
                           const std::vector<QSqlRecord>& records);
};

#endif /* end of include guard: QTTASKSTORAGEREADER_H_48BDKQZ7 */
