
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
#include "qt_storage_impl/QtWorkingDaysWriter.h"
#include "qt_storage_impl/DatabaseDescription.h"
#include "qt_storage_impl/utils/DateTimeConverter.h"
#include <iostream>

namespace sprint_timer::storage::qt_storage_impl {

QtWorkingDaysWriter::QtWorkingDaysWriter(DBService& dbService_)
    : dbService{dbService_}
{
    addExtraDaysQueryId
        = dbService.prepare(QString{"INSERT OR REPLACE INTO %1 (%2, %3) "
                                    "VALUES (:day, :is_workday);"}
                                .arg(ExtraDaysTable::name)
                                .arg(ExtraDaysTable::Columns::day)
                                .arg(ExtraDaysTable::Columns::workday));
    removeExtraDaysQueryId
        = dbService.prepare(QString{"DELETE FROM %1 WHERE %2 == (:day);"}
                                .arg(ExtraDaysTable::name)
                                .arg(ExtraDaysTable::Columns::day));
}

void QtWorkingDaysWriter::addExtraHolidays(const std::vector<dw::Date>& days)
{
    dbService.transaction();
    for (const auto& day : days) {
        dbService.bind(
            addExtraDaysQueryId,
            ":day",
            QVariant(QString::fromStdString(dw::to_string(day, "yyyy-MM-dd"))));
        dbService.bind(addExtraDaysQueryId, ":is_workday", QVariant(false));
        dbService.executePrepared(addExtraDaysQueryId);
    }
    dbService.commit();
}

void QtWorkingDaysWriter::addExtraWorkdays(const std::vector<dw::Date>& days)
{
    dbService.transaction();
    for (const auto& day : days) {
        dbService.bind(
            addExtraDaysQueryId,
            ":day",
            QVariant(QString::fromStdString(dw::to_string(day, "yyyy-MM-dd"))));
        dbService.bind(addExtraDaysQueryId, ":is_workday", QVariant(true));
        dbService.executePrepared(addExtraDaysQueryId);
    }
    dbService.commit();
}

void QtWorkingDaysWriter::removeExtraHolidays(const std::vector<dw::Date>& days)
{
    removeExtraDays(days);
}

void QtWorkingDaysWriter::removeExtraWorkdays(const std::vector<dw::Date>& days)
{
    removeExtraDays(days);
}

void QtWorkingDaysWriter::removeExtraDays(const std::vector<dw::Date>& days)
{
    dbService.transaction();
    for (const auto& day : days) {
        dbService.bind(
            removeExtraDaysQueryId,
            ":day",
            QVariant{QString::fromStdString(dw::to_string(day, "yyyy-MM-dd"))});
        dbService.executePrepared(removeExtraDaysQueryId);
    }
    dbService.commit();
}

} // namespace sprint_timer::storage::qt_storage_impl
