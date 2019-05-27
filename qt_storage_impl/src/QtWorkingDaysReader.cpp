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
#include "qt_storage_impl/QtWorkingDaysReader.h"
#include "qt_storage_impl/DatabaseDescription.h"
#include "qt_storage_impl/utils/DateTimeConverter.h"

namespace {

enum class ExceptionalDaysColumn { Date, Goal };

enum class ScheduleColumns {
    AppliedSince,
    MondayGoal,
    TuesdayGoal,
    WednesdayGoal,
    ThursdayGoal,
    FridayGoal,
    SaturdayGoal,
    SundayGoal
};

} // namespace

namespace sprint_timer::storage::qt_storage_impl {

QtWorkingDaysReader::QtWorkingDaysReader(DBService& dbService_,
                                         const IConfig& settings_)
    : dbService{dbService_}
    , settings{settings_}
{
    requestSchedulesQuery
        = QString("SELECT %1, %2, %3, %4, %5, %6, %7, %8 FROM %9;")
              .arg(ScheduleTable::Columns::applied_since)
              .arg(ScheduleTable::Columns::monday_goal)
              .arg(ScheduleTable::Columns::tuesday_goal)
              .arg(ScheduleTable::Columns::wednesday_goal)
              .arg(ScheduleTable::Columns::thursday_goal)
              .arg(ScheduleTable::Columns::friday_goal)
              .arg(ScheduleTable::Columns::saturday_goal)
              .arg(ScheduleTable::Columns::sunday_goal)
              .arg(ScheduleTable::name);
    requestExceptionalDaysQuery = QString("SELECT %1, %2 FROM %3;")
                                      .arg(ExceptionalDayTable::Columns::date)
                                      .arg(ExceptionalDayTable::Columns::goal)
                                      .arg(ExceptionalDayTable::name);

    connect(&dbService,
            &DBService::results,
            this,
            &QtWorkingDaysReader::onResultReceived);
}

void QtWorkingDaysReader::requestData(
    IWorkingDaysReader::ResultHandler resultHandler)
{
    contexts.push({resultHandler, WorkdayTracker{}});
    requestSchedulesQueryId = dbService.execute(requestSchedulesQuery);
}

void QtWorkingDaysReader::onResultReceived(
    qint64 queryId, const std::vector<QSqlRecord>& records)
{
    if (queryId == requestSchedulesQueryId) {
        for (const auto& rec : records) {
            const auto appliedSince = utils::DateTimeConverter::date(
                rec.value(static_cast<int>(ScheduleColumns::AppliedSince))
                    .toDate());
            const auto mon_goal
                = rec.value(static_cast<int>(ScheduleColumns::MondayGoal))
                      .toInt();
            const auto tue_goal
                = rec.value(static_cast<int>(ScheduleColumns::TuesdayGoal))
                      .toInt();
            const auto wed_goal
                = rec.value(static_cast<int>(ScheduleColumns::WednesdayGoal))
                      .toInt();
            const auto thu_goal
                = rec.value(static_cast<int>(ScheduleColumns::TuesdayGoal))
                      .toInt();
            const auto fri_goal
                = rec.value(static_cast<int>(ScheduleColumns::FridayGoal))
                      .toInt();
            const auto sat_goal
                = rec.value(static_cast<int>(ScheduleColumns::SaturdayGoal))
                      .toInt();
            const auto sun_goal
                = rec.value(static_cast<int>(ScheduleColumns::SundayGoal))
                      .toInt();
            WeekSchedule schedule;
            schedule.setTargetGoal(dw::Weekday::Monday, mon_goal);
            schedule.setTargetGoal(dw::Weekday::Tuesday, tue_goal);
            schedule.setTargetGoal(dw::Weekday::Wednesday, wed_goal);
            schedule.setTargetGoal(dw::Weekday::Thursday, thu_goal);
            schedule.setTargetGoal(dw::Weekday::Friday, fri_goal);
            schedule.setTargetGoal(dw::Weekday::Saturday, sat_goal);
            schedule.setTargetGoal(dw::Weekday::Sunday, sun_goal);
            contexts.front().tracker.addWeekSchedule(appliedSince, schedule);

            requestExceptionalDaysQueryId
                = dbService.execute(requestExceptionalDaysQuery);
        }
    }

    if (queryId == requestExceptionalDaysQueryId) {
        for (const auto& rec : records) {
            const auto goal
                = rec.value(static_cast<int>(ExceptionalDaysColumn::Goal))
                      .toInt();
            const auto date = utils::DateTimeConverter::date(
                rec.value(static_cast<int>(ExceptionalDaysColumn::Date))
                    .toDate());
            contexts.front().tracker.addExceptionalDay(date, goal);
        }
        auto [handler, tracker] = contexts.front();
        handler(std::move(tracker));
        // contexts.front().handler(contexts.front().tracker);
        contexts.pop();
    }
}

} // namespace sprint_timer::storage::qt_storage_impl
