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
#ifndef DAILYSTATISTICSHANDLER_H_NAHVBVYS
#define DAILYSTATISTICSHANDLER_H_NAHVBVYS

#include "core/QueryHandler.h"
#include "core/WorkScheduleReader.h"
#include "core/use_cases/request_schedule/WorkScheduleQuery.h"
#include "core/use_cases/request_statistics/DailyStatisticsQuery.h"
#include "core/use_cases/request_statistics/SprintStatisticsQuery.h"

namespace sprint_timer::use_cases {

class DailyStatisticsHandler : public QueryHandler<DailyStatisticsQuery> {
public:
    using request_statistics_handler_t = QueryHandler<SprintStatisticsQuery>;
    using work_schedule_handler_t = QueryHandler<WorkScheduleQuery>;

    explicit DailyStatisticsHandler(
        work_schedule_handler_t& workScheduleHandler,
        request_statistics_handler_t& sprintStatisticsHandler);

    result_t handle(DailyStatisticsQuery&& query) override;

private:
    work_schedule_handler_t& workScheduleHandler;
    request_statistics_handler_t& sprintStatisticsHandler;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: DAILYSTATISTICSHANDLER_H_NAHVBVYS */
