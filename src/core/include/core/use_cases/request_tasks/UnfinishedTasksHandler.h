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
#ifndef UNFINISHEDTASKSHANDLER_H_LVO6P2GE
#define UNFINISHEDTASKSHANDLER_H_LVO6P2GE

#include "core/TaskStorageReader.h"
#include "core/QueryHandler.h"
#include "core/use_cases/request_tasks/UnfinishedTasksQuery.h"

namespace sprint_timer::use_cases {

class UnfinishedTasksHandler
    : public QueryHandler<UnfinishedTasksQuery, std::vector<entities::Task>> {
public:
    explicit UnfinishedTasksHandler(TaskStorageReader& reader);

    std::vector<entities::Task> handle(UnfinishedTasksQuery&&) override;

private:
    TaskStorageReader& reader;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: UNFINISHEDTASKSHANDLER_H_LVO6P2GE */
