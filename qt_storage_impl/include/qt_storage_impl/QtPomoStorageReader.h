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
#ifndef QTSQLITEPOMODOROSTORAGEREADER_H_JXULCJ6I
#define QTSQLITEPOMODOROSTORAGEREADER_H_JXULCJ6I

#include "core/IPomodoroStorageReader.h"
#include "core/entities/Tag.h"
#include "qt_storage_impl/db_service.h"
#include <QObject>


class QtPomoStorageReader : public QObject, public IPomodoroStorageReader {
    Q_OBJECT

public:
    explicit QtPomoStorageReader(DBService& dbService);

    void requestItems(const TimeSpan& timeSpan, Handler handler) final;

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
    long long mQueryId{-1};
    DBService& dbService;
    std::list<Handler> handler_queue;

    Pomodoro pomodoroFromQSqlRecord(const QSqlRecord& record);

    QVariant columnData(const QSqlRecord& record, Columns column);

private slots:
    void onResultsReceived(long long queryId,
                           const std::vector<QSqlRecord>& records);
};


#endif /* end of include guard: QTSQLITEPOMODOROSTORAGEREADER_H_JXULCJ6I */
