
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
#include "qt_storage_impl/QtWorkingDaysWriter.h"
#include "qt_storage_impl/DatabaseDescription.h"
#include "qt_storage_impl/utils/DateTimeConverter.h"

#include <iostream>

namespace sprint_timer::storage::qt_storage_impl {

QtWorkingDaysWriter::QtWorkingDaysWriter(DBService& dbService_)
    : dbService{dbService_}
{
    storeSchedulesQueryId = dbService.prepare(
        QString{"INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8, %9) VALUES ( "
                ":applied_since, :mon, :tue, :wed, :thu, :fri, :sat, :sun);"}
            .arg(ScheduleTable::name)
            .arg(ScheduleTable::Columns::applied_since)
            .arg(ScheduleTable::Columns::monday_goal)
            .arg(ScheduleTable::Columns::tuesday_goal)
            .arg(ScheduleTable::Columns::wednesday_goal)
            .arg(ScheduleTable::Columns::thursday_goal)
            .arg(ScheduleTable::Columns::friday_goal)
            .arg(ScheduleTable::Columns::saturday_goal)
            .arg(ScheduleTable::Columns::sunday_goal));
    storeExceptionalDaysQuery = dbService.prepare(
        QString{"INSERT INTO %1 (%2, %3) VALUES (:date, :goal);"}
            .arg(ExceptionalDayTable::name)
            .arg(ExceptionalDayTable::Columns::date)
            .arg(ExceptionalDayTable::Columns::goal));
}

void QtWorkingDaysWriter::changeWorkingDays(const WorkdayTracker& tracker)
{
    dbService.transaction();

    dbService.execute(
        QString{"DELETE FROM %1;"}.arg(ExceptionalDayTable::name));
    dbService.execute(QString{"DELETE FROM %1;"}.arg(ScheduleTable::name));

    for (const auto& [date, schedule] : tracker.scheduleRoaster()) {
        dbService.bind(
            storeSchedulesQueryId,
            ":applied_since",
            QString::fromStdString(dw::to_string(date, "yyyy-MM-dd")));
        dbService.bind(storeSchedulesQueryId,
                       ":mon",
                       schedule.targetGoal(dw::Weekday::Monday));
        dbService.bind(storeSchedulesQueryId,
                       ":tue",
                       schedule.targetGoal(dw::Weekday::Tuesday));
        dbService.bind(storeSchedulesQueryId,
                       ":wed",
                       schedule.targetGoal(dw::Weekday::Wednesday));
        dbService.bind(storeSchedulesQueryId,
                       ":thu",
                       schedule.targetGoal(dw::Weekday::Thursday));
        dbService.bind(storeSchedulesQueryId,
                       ":fri",
                       schedule.targetGoal(dw::Weekday::Friday));
        dbService.bind(storeSchedulesQueryId,
                       ":sat",
                       schedule.targetGoal(dw::Weekday::Saturday));
        dbService.bind(storeSchedulesQueryId,
                       ":sun",
                       schedule.targetGoal(dw::Weekday::Sunday));
        dbService.executePrepared(storeSchedulesQueryId);
    }

    const auto holidays = tracker.extraHolidays();
    const auto workdays = tracker.extraWorkdays();
    for (const auto& day : holidays) {
        dbService.bind(
            storeExceptionalDaysQuery,
            ":date",
            QString::fromStdString(dw::to_string(day, "yyyy-MM-dd")));
        dbService.bind(storeExceptionalDaysQuery, ":goal", 0);
        dbService.executePrepared(storeExceptionalDaysQuery);
    }
    for (const auto& day : workdays) {
        dbService.bind(
            storeExceptionalDaysQuery,
            ":date",
            QString::fromStdString(dw::to_string(day, "yyyy-MM-dd")));
        dbService.bind(storeExceptionalDaysQuery, ":goal", tracker.goal(day));
        dbService.executePrepared(storeExceptionalDaysQuery);
    }

    dbService.commit();
}

void QtWorkingDaysWriter::addExtraHolidays(const std::vector<dw::Date>& days)
{
    // dbService.transaction();
    // for (const auto& day : days) {
    //     dbService.bind(
    //         addExtraDaysQueryId,
    //         ":day",
    //         QVariant(QString::fromStdString(dw::to_string(day,
    //         "yyyy-MM-dd"))));
    //     dbService.bind(addExtraDaysQueryId, ":is_workday", QVariant(false));
    //     dbService.executePrepared(addExtraDaysQueryId);
    // }
    // dbService.commit();
}

void QtWorkingDaysWriter::addExtraWorkdays(const std::vector<dw::Date>& days)
{
    // dbService.transaction();
    // for (const auto& day : days) {
    //     dbService.bind(
    //         addExtraDaysQueryId,
    //         ":day",
    //         QVariant(QString::fromStdString(dw::to_string(day,
    //         "yyyy-MM-dd"))));
    //     dbService.bind(addExtraDaysQueryId, ":is_workday", QVariant(true));
    //     dbService.executePrepared(addExtraDaysQueryId);
    // }
    // dbService.commit();
}

void QtWorkingDaysWriter::removeExtraHolidays(const std::vector<dw::Date>& days)
{
    // removeExtraDays(days);
}

void QtWorkingDaysWriter::removeExtraWorkdays(const std::vector<dw::Date>& days)
{
    // removeExtraDays(days);
}

void QtWorkingDaysWriter::removeExtraDays(const std::vector<dw::Date>& days)
{
    // dbService.transaction();
    // for (const auto& day : days) {
    //     dbService.bind(
    //         removeExtraDaysQueryId,
    //         ":day",
    //         QVariant{QString::fromStdString(dw::to_string(day,
    //         "yyyy-MM-dd"))});
    //     dbService.executePrepared(removeExtraDaysQueryId);
    // }
    // dbService.commit();
}

} // namespace sprint_timer::storage::qt_storage_impl
