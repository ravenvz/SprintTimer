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
#ifndef TOPTAGFREQUENCIESHANDLER_H_NCSZNM4P
#define TOPTAGFREQUENCIESHANDLER_H_NCSZNM4P

#include "api/com_query/QueryHandler.h"
#include "api/requests/SprintStatisticsQuery.h"
#include "api/requests/TopTagFrequenciesQuery.h"

namespace sprint_timer::api {

class TopTagFrequenciesHandler
    : public asp::QueryHandler<TopTagFrequenciesQuery> {
public:
    using sprint_statistics_handler_t =
        asp::QueryHandler<SprintStatisticsQuery>;

    explicit TopTagFrequenciesHandler(
        sprint_statistics_handler_t& sprintStatisticsHandler);

    TopTagFrequenciesQuery::Result
    handle(const TopTagFrequenciesQuery& query) override;

private:
    sprint_statistics_handler_t& sprintStatisticsHandler;
};

} // namespace sprint_timer::api

#endif /* end of include guard: TOPTAGFREQUENCIESHANDLER_H_NCSZNM4P */
