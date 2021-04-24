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
#ifndef FINISHEDTASKSQUERYHANDLERSPECIALIZATION_H_RAKVEY9S
#define FINISHEDTASKSQUERYHANDLERSPECIALIZATION_H_RAKVEY9S

#include "CacheAwareQueryHandler.h"
#include "core/use_cases/request_tasks/FinishedTasksQuery.h"

namespace sprint_timer::compose {

template <>
typename use_cases::FinishedTasksQuery::result_t
CacheAwareQueryHandler<use_cases::FinishedTasksQuery>::handle(
    use_cases::FinishedTasksQuery&& query)
{
    if (!cachedResult ||
        (cachedQuery && (query.dateRange != cachedQuery->dateRange))) {
        cachedQuery = query;
        cachedResult = wrapped->handle(std::move(query));
    }
    return *cachedResult;
}

} // namespace sprint_timer::compose

#endif /* end of include guard:                                                \
          FINISHEDTASKSQUERYHANDLERSPECIALIZATION_H_RAKVEY9S */
