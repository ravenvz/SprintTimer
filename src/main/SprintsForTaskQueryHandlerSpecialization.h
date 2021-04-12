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
#ifndef SPRINTSFORTASKQUERYHANDLERSPECIALIZATION_H_R4ADYXNW
#define SPRINTSFORTASKQUERYHANDLERSPECIALIZATION_H_R4ADYXNW

#include "CacheAwareQueryHandler.h"
#include "core/use_cases/request_sprints/SprintsForTaskQuery.h"

namespace sprint_timer::compose {

template <>
std::vector<entities::Sprint>
CacheAwareQueryHandler<use_cases::SprintsForTaskQuery,
                       std::vector<entities::Sprint>>::
    handle(use_cases::SprintsForTaskQuery&& query)
{
    if (!cachedResult ||
        (cachedQuery && (query.taskUuid != cachedQuery->taskUuid))) {
        cachedQuery = query;
        cachedResult = wrapped->handle(std::move(query));
    }
    return *cachedResult;
}

} // namespace sprint_timer::compose

#endif /* end of include guard:                                                \
          SPRINTSFORTASKQUERYHANDLERSPECIALIZATION_H_R4ADYXNW */
