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
#include "qt_storage/QtWorkingDaysStorage.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/TransactionGuard.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include "qt_storage/utils/QueryUtils.h"

namespace {

using namespace sprint_timer::storage::qt_storage;

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

std::pair<dw::Date, sprint_timer::WeekSchedule>
recordToSchedule(const QSqlRecord& record);

std::pair<dw::Date, int> recordToExceptionalDay(const QSqlRecord& record);

const QString readSchedule =
    QString("SELECT %1, %2, %3, %4, %5, %6, %7, %8 FROM %9;")
        .arg(ScheduleTable::Columns::applied_since)
        .arg(ScheduleTable::Columns::monday_goal)
        .arg(ScheduleTable::Columns::tuesday_goal)
        .arg(ScheduleTable::Columns::wednesday_goal)
        .arg(ScheduleTable::Columns::thursday_goal)
        .arg(ScheduleTable::Columns::friday_goal)
        .arg(ScheduleTable::Columns::saturday_goal)
        .arg(ScheduleTable::Columns::sunday_goal)
        .arg(ScheduleTable::name);

const QString readExceptionalDays = QString("SELECT %1, %2 FROM %3;")
                                        .arg(ExceptionalDayTable::Columns::date)
                                        .arg(ExceptionalDayTable::Columns::goal)
                                        .arg(ExceptionalDayTable::name);

const QString deleteExceptionalDaysStr =
    QString{"DELETE FROM %1;"}.arg(ExceptionalDayTable::name);

const QString deleteAllSchedulesStr =
    QString{"DELETE FROM %1;"}.arg(ScheduleTable::name);

} // namespace

namespace sprint_timer::storage::qt_storage {

QtWorkScheduleStorage::QtWorkScheduleStorage(const QString& connectionName_)
    : connectionName{connectionName_}
{
    using namespace qt_storage;

    storeScheduleQuery = tryPrepare(
        connectionName,
        QString{"INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8, %9) VALUES ( "
                ":applied_since, :mon, :tue, :wed, :thu, :fri, :sat, "
                ":sun);"}
            .arg(ScheduleTable::name)
            .arg(ScheduleTable::Columns::applied_since)
            .arg(ScheduleTable::Columns::monday_goal)
            .arg(ScheduleTable::Columns::tuesday_goal)
            .arg(ScheduleTable::Columns::wednesday_goal)
            .arg(ScheduleTable::Columns::thursday_goal)
            .arg(ScheduleTable::Columns::friday_goal)
            .arg(ScheduleTable::Columns::saturday_goal)
            .arg(ScheduleTable::Columns::sunday_goal));

    storeExceptionalDaysQuery =
        tryPrepare(connectionName,
                   QString{"INSERT INTO %1 (%2, %3) VALUES (:date, :goal);"}
                       .arg(ExceptionalDayTable::name)
                       .arg(ExceptionalDayTable::Columns::date)
                       .arg(ExceptionalDayTable::Columns::goal));
}

void QtWorkScheduleStorage::updateSchedule(const WorkSchedule& schedule)
{
    TransactionGuard guard{connectionName};

    QSqlQuery deleteAllExceptionalDays{QSqlDatabase::database(connectionName)};
    tryExecute(deleteAllExceptionalDays, deleteExceptionalDaysStr);

    QSqlQuery deleteAllSchedules{QSqlDatabase::database(connectionName)};
    tryExecute(deleteAllSchedules, deleteAllSchedulesStr);

    for (const auto& [date, schedule] : schedule.roaster()) {
        storeScheduleQuery.bindValue(
            ":applied_since",
            QString::fromStdString(dw::to_string(date, "yyyy-MM-dd")));
        storeScheduleQuery.bindValue(":mon",
                                     schedule.targetGoal(dw::Weekday::Monday));
        storeScheduleQuery.bindValue(":tue",
                                     schedule.targetGoal(dw::Weekday::Tuesday));
        storeScheduleQuery.bindValue(
            ":wed", schedule.targetGoal(dw::Weekday::Wednesday));
        storeScheduleQuery.bindValue(
            ":thu", schedule.targetGoal(dw::Weekday::Thursday));
        storeScheduleQuery.bindValue(":fri",
                                     schedule.targetGoal(dw::Weekday::Friday));
        storeScheduleQuery.bindValue(
            ":sat", schedule.targetGoal(dw::Weekday::Saturday));
        storeScheduleQuery.bindValue(":sun",
                                     schedule.targetGoal(dw::Weekday::Sunday));
        tryExecute(storeScheduleQuery);
    }

    const auto exceptionalDays = schedule.exceptionalDays();
    for (const auto& [day, goal] : exceptionalDays) {
        storeExceptionalDaysQuery.bindValue(
            ":date", QString::fromStdString(dw::to_string(day, "yyyy-MM-dd")));
        storeExceptionalDaysQuery.bindValue(":goal", goal);
        tryExecute(storeExceptionalDaysQuery);
    }

    guard.commit();
}

WorkSchedule QtWorkScheduleStorage::schedule()
{
    QSqlQuery readScheduleQuery{QSqlDatabase::database(connectionName)};
    tryExecute(readScheduleQuery, readSchedule);

    WorkSchedule workSchedule;

    const auto storedSchedules = copyAllRecords(readScheduleQuery);

    for (const auto& rec : storedSchedules) {
        const auto& [appliedSince, schedule] = recordToSchedule(rec);
        workSchedule.addWeekSchedule(appliedSince, schedule);
    }

    QSqlQuery readExceptionalDaysQuery{QSqlDatabase::database(connectionName)};
    tryExecute(readExceptionalDaysQuery, readExceptionalDays);

    const auto exceptionalDayRecords = copyAllRecords(readExceptionalDaysQuery);

    for (const auto& rec : exceptionalDayRecords) {
        const auto& [date, goal] = recordToExceptionalDay(rec);
        workSchedule.addExceptionalDay(date, goal);
    }

    return workSchedule;
}

} // namespace sprint_timer::storage::qt_storage

namespace {

std::pair<dw::Date, sprint_timer::WeekSchedule>
recordToSchedule(const QSqlRecord& record)
{
    using sprint_timer::storage::utils::DateTimeConverter;
    const auto appliedSince = DateTimeConverter::date(
        record.value(static_cast<int>(ScheduleColumns::AppliedSince)).toDate());
    const auto mon_goal =
        record.value(static_cast<int>(ScheduleColumns::MondayGoal)).toInt();
    const auto tue_goal =
        record.value(static_cast<int>(ScheduleColumns::TuesdayGoal)).toInt();
    const auto wed_goal =
        record.value(static_cast<int>(ScheduleColumns::WednesdayGoal)).toInt();
    const auto thu_goal =
        record.value(static_cast<int>(ScheduleColumns::TuesdayGoal)).toInt();
    const auto fri_goal =
        record.value(static_cast<int>(ScheduleColumns::FridayGoal)).toInt();
    const auto sat_goal =
        record.value(static_cast<int>(ScheduleColumns::SaturdayGoal)).toInt();
    const auto sun_goal =
        record.value(static_cast<int>(ScheduleColumns::SundayGoal)).toInt();
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
    const auto goal =
        record.value(static_cast<int>(ExceptionalDaysColumn::Goal)).toInt();
    const auto date = DateTimeConverter::date(
        record.value(static_cast<int>(ExceptionalDaysColumn::Date)).toDate());

    return {date, goal};
}

} // namespace
