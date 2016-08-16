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
#ifndef QTSQLITEPOMODOROYEARRANGEREADER_H_6LK9HHEM
#define QTSQLITEPOMODOROYEARRANGEREADER_H_6LK9HHEM

#include "core/IYearRangeReader.h"
#include "qt_storage_impl/db_service.h"
#include <QObject>
#include <functional>

class QtPomoYearRangeReader : public QObject, public IYearRangeReader {
    Q_OBJECT

public:
    QtPomoYearRangeReader(DBService& dbService);

    void requestYearRange(Handler handler) final;

private:
    DBService& dbService;
    Handler handler;
    long long mQueryId{-1};

private slots:
    void onResultsReceived(long long queryId,
                           const std::vector<QSqlRecord>& records);
};


#endif /* end of include guard: QTSQLITEPOMODOROYEARRANGEREADER_H_6LK9HHEM */
