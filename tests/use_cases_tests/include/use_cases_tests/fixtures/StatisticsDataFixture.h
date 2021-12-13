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
#ifndef STATISTICSDATAFIXTURE_H_OI1Y9IVE
#define STATISTICSDATAFIXTURE_H_OI1Y9IVE

#include "core/CommandHandler.h"
#include "core/use_cases/create_task/CreateTaskCommand.h"
#include "core/use_cases/register_sprint/RegisterSprintBulkCommand.h"

namespace fixtures {

/*  Creates sprints with following distribution
 * in date range {15.01.2022, 15.01.2022 + 10 days}.
 *
 * Function returns this datetime range for convinience.
 *
 *  3   0   2   5   2   3   1   3   1   2   2
 * Sat Sun Mon Tue Wed Thu Fri Sat Sun Mon Tue
 *
 * Represent a day partition
 *
 * Day has 6 4-hour parts:
 *      Midnight  22:00 - 2:00   2
 *      Night      2:00 - 6:00   4
 *      Morning    6:00 - 10:00  5
 *      Noon      10:00 - 14:00  5
 *      Afternoon 14:00 - 18:00  3
 *      Evening   18:00 - 22:00  5
 */

dw::DateTimeRange givenStatisticsDataFixtureLoaded(
    sprint_timer::CommandHandler<sprint_timer::use_cases::CreateTaskCommand>&
        createTaskHandler,
    sprint_timer::CommandHandler<
        sprint_timer::use_cases::RegisterSprintBulkCommand>&
        registerSprintsHandler);

} // namespace fixtures

#endif /* end of include guard: STATISTICSDATAFIXTURE_H_OI1Y9IVE */
