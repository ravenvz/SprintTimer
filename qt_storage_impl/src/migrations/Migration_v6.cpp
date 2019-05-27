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
#include "qt_storage_impl/migrations/Migration_v6.h"
#include "qt_storage_impl/utils/QueryUtils.h"
#include <QDate>
#include <QString>

namespace sprint_timer::storage::qt_storage_impl {

void Migration_v6::run(QSqlDatabase& database) const
{
    QSqlQuery query{database};
    tryExecute(query,
               "CREATE TABLE exceptional_day (id INTEGER PRIMARY KEY "
               "AUTOINCREMENT, date DATE UNIQUE, goal INTEGER);");
    tryExecute(query,
               "CREATE TABLE schedule (id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "applied_since DATE UNIQUE, mon_goal INTEGER, tue_goal INTEGER, "
               "wed_goal INTEGER, thu_goal INTEGER, fri_goal INTEGER, sat_goal "
               "INTEGER, sun_goal INTEGER);");
    const int goal{13};
    tryExecute(query,
               QString{"INSERT INTO schedule (applied_since, mon_goal, "
                       "tue_goal, wed_goal, thu_goal, fri_goal, sat_goal, "
                       "sun_goal) VALUES ((SELECT date(min(start_time)) FROM "
                       "sprint), %2, %2, %2, %2, %2, 0, 0);"}
                   .arg(goal));
}

} // namespace sprint_timer::storage::qt_storage_impl
