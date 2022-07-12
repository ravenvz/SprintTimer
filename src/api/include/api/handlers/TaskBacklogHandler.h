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
#ifndef TASKBACKLOGHANDLER_H_6JINIZUV
#define TASKBACKLOGHANDLER_H_6JINIZUV

#include "api/com_query/QueryHandler.h"
#include "api/requests/TaskBacklogQuery.h"
#include "core/TaskStorageReader.h"

namespace sprint_timer::api {

class TaskBacklogHandler : public asp::QueryHandler<TaskBacklogQuery> {
public:
    explicit TaskBacklogHandler(TaskStorageReader& taskReader);

    typename TaskBacklogQuery::result_t
    handle(TaskBacklogQuery&& query) override;

private:
    TaskStorageReader& taskReader;
};

inline TaskBacklogHandler::TaskBacklogHandler(TaskStorageReader& taskReader_)
    : taskReader{taskReader_}
{
}

inline typename TaskBacklogQuery::result_t
TaskBacklogHandler::handle(const TaskBacklogQuery& /* query */)
{
    return TaskBacklog{taskReader.unfinishedTasks()};
}

} // namespace sprint_timer::api

#endif /* end of include guard: TASKBACKLOGHANDLER_H_6JINIZUV */
