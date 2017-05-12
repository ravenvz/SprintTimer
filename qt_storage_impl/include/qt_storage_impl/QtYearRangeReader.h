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
#ifndef QTYEARRANGEREADER_H_6LK9HHEM
#define QTYEARRANGEREADER_H_6LK9HHEM

#include "core/IYearRangeReader.h"
#include "qt_storage_impl/DBService.h"
#include <QObject>
#include <functional>

class QtYearRangeReader : public QObject, public IYearRangeReader {
    Q_OBJECT

public:
    QtYearRangeReader(DBService& dbService);

    void requestYearRange(Handler handler) final;

private:
    DBService& dbService;
    Handler handler;
    long long mQueryId{-1};

private slots:
    void onResultsReceived(long long queryId,
                           const std::vector<QSqlRecord>& records);
};


#endif /* end of include guard: QTYEARRANGEREADER_H_6LK9HHEM */
