/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "qt_storage_impl/QtWorkingDaysReader.h"
#include "qt_storage_impl/DatabaseDescription.h"
#include "qt_storage_impl/utils/DateTimeConverter.h"

namespace sprint_timer::storage::qt_storage_impl {

QtWorkingDaysReader::QtWorkingDaysReader(DBService& dbService_,
                                         const IConfig& settings_)
    : dbService{dbService_}
    , settings{settings_}
{
    requestExtraDaysQuery = QString("SELECT %1, %2 FROM %3;")
                                .arg(ExtraDaysTable::Columns::day)
                                .arg(ExtraDaysTable::Columns::workday)
                                .arg(ExtraDaysTable::name);
    connect(&dbService,
            &DBService::results,
            this,
            &QtWorkingDaysReader::onResultReceived);
}

void QtWorkingDaysReader::requestData(
    IWorkingDaysReader::ResultHandler resultHandler)
{
    handler = resultHandler;
    requestExtraDaysQueryId = dbService.execute(requestExtraDaysQuery);
    // using namespace dw;
    // std::cout << "QtWorkingDaysReader stub requsting data" << std::endl;
    // WorkdayTracker tracker{utils::WeekdaySelection{31}};
    // tracker.addExtraHoliday(dw::Date{Year{2019}, Month{3}, Day{8}});
    // tracker.addExtraHoliday(dw::Date{Year{2019}, Month{1}, Day{1}});
    // tracker.addExtraHoliday(dw::Date{Year{2019}, Month{1}, Day{2}});
    // tracker.addExtraHoliday(dw::Date{Year{2019}, Month{1}, Day{3}});
    // tracker.addExtraHoliday(dw::Date{Year{2019}, Month{1}, Day{4}});
    // tracker.addExtraHoliday(dw::Date{Year{2019}, Month{1}, Day{7}});
    // tracker.addExtraHoliday(dw::Date{Year{2019}, Month{1}, Day{8}});
    //
    // resultHandler(tracker);
}

void QtWorkingDaysReader::onResultReceived(
    qint64 queryId, const std::vector<QSqlRecord>& records)
{
    if (queryId != requestExtraDaysQueryId)
        return;

    WorkdayTracker tracker{settings.workdays()};
    for (const auto& rec : records) {
        const auto isWorkday
            = rec.value(static_cast<int>(Column::Workday)).toBool();
        const auto date = utils::DateTimeConverter::date(
            rec.value(static_cast<int>(Column::Day)).toDate());
        isWorkday ? tracker.addExtraWorkday(date)
                  : tracker.addExtraHoliday(date);
    }
    handler(tracker);
}

} // namespace sprint_timer::storage::qt_storage_impl
