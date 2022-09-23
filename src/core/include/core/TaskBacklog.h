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
#ifndef TASKBACKLOG_H_HSDVVDUC
#define TASKBACKLOG_H_HSDVVDUC

#include "core/Task.h"

namespace sprint_timer {

class TaskBacklog {
public:
    TaskBacklog(std::vector<Task> tasks_)
        : tasks{std::move(tasks_)}
    {
    }

    bool operator==(const TaskBacklog&) const = default;

private:
    std::vector<Task> tasks;
};

} // namespace sprint_timer

#endif /* end of include guard: TASKBACKLOG_H_HSDVVDUC */
