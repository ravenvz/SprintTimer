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
#include "qt_storage_impl/QtWorkingDaysStorage.h"
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

std::pair<dw::Date, sprint_timer::WeekSchedule> recordToSchedule(const QSqlRecord& record);

std::pair<dw::Date, int> recordToExceptionalDay(const QSqlRecord& record);

} // namespace


namespace sprint_timer::storage::qt_storage_impl {

QtWorkingDaysStorage::QtWorkingDaysStorage(DBService& dbService_, QObject* parent_)
    : QObject{parent_}
    , dbService{dbService_}
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
            &QtWorkingDaysStorage::onResultReceived);
}

void QtWorkingDaysStorage::changeWorkingDays(const WorkdayTracker& tracker)
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

    const auto exceptionalDays = tracker.exceptionalDays();
    for (const auto& [day, goal] : exceptionalDays) {
        dbService.bind(
            storeExceptionalDaysQuery,
            ":date",
            QString::fromStdString(dw::to_string(day, "yyyy-MM-dd")));
        dbService.bind(storeExceptionalDaysQuery, ":goal", goal);
        dbService.executePrepared(storeExceptionalDaysQuery);
    }

    dbService.commit();
}

void QtWorkingDaysStorage::requestData(
    IWorkingDaysReader::ResultHandler resultHandler)
{
    contexts.push({resultHandler, WorkdayTracker{}});
    requestSchedulesQueryId = dbService.execute(requestSchedulesQuery);
}

void QtWorkingDaysStorage::onResultReceived(
    qint64 queryId, const std::vector<QSqlRecord>& records)
{
    if (queryId == requestSchedulesQueryId) {

        for (const auto& rec : records) {
            const auto& [appliedSince, schedule] = recordToSchedule(rec);
            contexts.front().tracker.addWeekSchedule(appliedSince, schedule);
        }

        requestExceptionalDaysQueryId
            = dbService.execute(requestExceptionalDaysQuery);
    }

    if (queryId == requestExceptionalDaysQueryId) {
        for (const auto& rec : records) {
            const auto& [date, goal] = recordToExceptionalDay(rec);
            contexts.front().tracker.addExceptionalDay(date, goal);
        }
        auto [handler, tracker] = contexts.front();
        handler(std::move(tracker));
        contexts.pop();
    }
}

} // namespace sprint_timer::storage::qt_storage_impl

namespace  {

std::pair<dw::Date, sprint_timer::WeekSchedule> recordToSchedule(const QSqlRecord& record)
{
    using sprint_timer::storage::utils::DateTimeConverter;
    const auto appliedSince = DateTimeConverter::date(
            record.value(static_cast<int>(ScheduleColumns::AppliedSince))
                    .toDate());
    const auto mon_goal
            = record.value(static_cast<int>(ScheduleColumns::MondayGoal))
                    .toInt();
    const auto tue_goal
            = record.value(static_cast<int>(ScheduleColumns::TuesdayGoal))
                    .toInt();
    const auto wed_goal
            = record.value(static_cast<int>(ScheduleColumns::WednesdayGoal))
                    .toInt();
    const auto thu_goal
            = record.value(static_cast<int>(ScheduleColumns::TuesdayGoal))
                    .toInt();
    const auto fri_goal
            = record.value(static_cast<int>(ScheduleColumns::FridayGoal))
                    .toInt();
    const auto sat_goal
            = record.value(static_cast<int>(ScheduleColumns::SaturdayGoal))
                    .toInt();
    const auto sun_goal
            = record.value(static_cast<int>(ScheduleColumns::SundayGoal))
                    .toInt();
    sprint_timer::WeekSchedule schedule;
    schedule.setTargetGoal(dw::Weekday::Monday, mon_goal);
    schedule.setTargetGoal(dw::Weekday::Tuesday, tue_goal);
    schedule.setTargetGoal(dw::Weekday::Wednesday, wed_goal);
    schedule.setTargetGoal(dw::Weekday::Thursday, thu_goal);
    schedule.setTargetGoal(dw::Weekday::Friday, fri_goal);
    schedule.setTargetGoal(dw::Weekday::Saturday, sat_goal);
    schedule.setTargetGoal(dw::Weekday::Sunday, sun_goal);

    return {appliedSince, schedule};
}

std::pair<dw::Date, int> recordToExceptionalDay(const QSqlRecord& record)
{
    using sprint_timer::storage::utils::DateTimeConverter;
    const auto goal
        = record.value(static_cast<int>(ExceptionalDaysColumn::Goal))
              .toInt();
    const auto date = DateTimeConverter::date(
        record.value(static_cast<int>(ExceptionalDaysColumn::Date))
            .toDate());

    return {date, goal};
}

} // namespace
