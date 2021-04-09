/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "qt_storage/QtOperationalRangeReader.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include "qt_storage/utils/QueryUtils.h"

namespace sprint_timer::storage::qt_storage {

QtOperationalRangeReader::QtOperationalRangeReader(
    const QString& connectionName_)
    : connectionName{connectionName_}
{
}

dw::DateRange QtOperationalRangeReader::operationalRange()
{
    using namespace qt_storage;
    QSqlQuery query{QSqlDatabase::database(connectionName)};
    tryExecute(query,
               QString{"SELECT date(min(%1)), date(max(%1)) FROM %2;"}
                   .arg(SprintTable::Columns::startTime)
                   .arg(SprintTable::name));
    if (query.next()) {
        const dw::DateRange range{
            utils::DateTimeConverter::date(query.record().value(0).toDate()),
            std::max(dw::current_date_local(),
                     utils::DateTimeConverter::date(
                         query.record().value(1).toDate()))};
        query.finish();
        return range;
    }
    else {
        query.finish();
        return dw::DateRange{dw::current_date_local(),
                             dw::current_date_local()};
    }
}

} // namespace sprint_timer::storage::qt_storage
