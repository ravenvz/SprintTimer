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
#ifndef WORKDAYSTATISTICSHANDLER_H_CKHU2K7U
#define WORKDAYSTATISTICSHANDLER_H_CKHU2K7U

#include "api/com_query/QueryHandler.h"
#include "api/handlers/SprintStatisticsHandler.h"
#include "api/requests/SprintStatisticsQuery.h"
#include "api/requests/WorkdayStatisticsQuery.h"

namespace sprint_timer::api {

class WorkdayStatisticsHandler
    : public asp::QueryHandler<WorkdayStatisticsQuery> {
public:
    using sprint_statistics_handler_t =
        asp::QueryHandler<SprintStatisticsQuery>;

    explicit WorkdayStatisticsHandler(sprint_statistics_handler_t& handler);

    WorkdayStatisticsQuery::Result
    handle(const WorkdayStatisticsQuery& query) override;

private:
    sprint_statistics_handler_t& handler;
};

} // namespace sprint_timer::api

#endif /* end of include guard: WORKDAYSTATISTICSHANDLER_H_CKHU2K7U */

