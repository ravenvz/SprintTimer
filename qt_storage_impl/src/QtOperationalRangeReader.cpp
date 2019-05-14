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
#include "qt_storage_impl/QtOperationalRangeReader.h"
#include "qt_storage_impl/DatabaseDescription.h"
#include "qt_storage_impl/utils/DateTimeConverter.h"

namespace sprint_timer::storage::qt_storage_impl {

QtOperationalRangeReader::QtOperationalRangeReader(DBService& dbService)
    : dbService{dbService}
{
    connect(&dbService,
            &DBService::results,
            this,
            &QtOperationalRangeReader::onResultsReceived);
}

void QtOperationalRangeReader::requestOperationalRange(Handler handler)
{
    qint64 queryId = dbService.execute(
        QString{"SELECT date(min(%1)), date(max(%1)) FROM %2;"}
            .arg(SprintTable::Columns::startTime)
            .arg(SprintTable::name));
    handlers.insert(std::make_pair(queryId, handler));
}

void QtOperationalRangeReader::onResultsReceived(
    qint64 queryId, const std::vector<QSqlRecord>& records)
{
    if (auto it = handlers.find(queryId); it != handlers.end()) {
        // TODO should signal error
        if (records.empty())
            return;
        const auto& record = records.front();
        const dw::DateRange range{
            utils::DateTimeConverter::date(record.value(0).toDate()),
            utils::DateTimeConverter::date(record.value(1).toDate())};
        it->second(range);
        handlers.erase(queryId);
    }
}

} // namespace sprint_timer::storage::qt_storage_impl

