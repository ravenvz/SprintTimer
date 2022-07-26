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
#ifndef TASKSINRANGEHANDLER_H_XJYQDCR4
#define TASKSINRANGEHANDLER_H_XJYQDCR4

#include "api/com_query/QueryHandler.h"
#include "api/TaskStorageReader.h"
// #include "core/use_cases/TaskMapper.h"
#include "api/com_query/TasksInRangeQuery.h"

namespace sprint_timer::api {

class TasksInRangeHandler : public asp::QueryHandler<TasksInRangeQuery> {
public:
    explicit TasksInRangeHandler(TaskStorageReader& taskReader_)
        : reader{taskReader_}
    {
    }

    typename TasksInRangeQuery::result_t handle(const TasksInRangeQuery& query)
    {
        return makeDTOs(reader.allTasks(query.dateRange));
    }

private:
    TaskStorageReader& reader;
};

} // namespace sprint_timer::api

#endif /* end of include guard: TASKSINRANGEHANDLER_H_XJYQDCR4 */
