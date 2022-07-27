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
#ifndef TASK_H_7VXCYMOK
#define TASK_H_7VXCYMOK

#include "core/GoalProgress.h"
#include "core/Sprint.h"
#include "core/Tag.h"
#include "core/utils/StringUtils.h"
#include "date_wrapper/date_wrapper.h"
#include <vector>

namespace sprint_timer {

/* Represent Task that may have many associated sprints.
 *
 * Task has name and tags and might be either in completed or
 * non-completed state. It also has an estimated const in sprints
 * and a number of sprints that were actually spent on this Task. */
class Task {

public:
    Task(std::string name,
         int estimatedCost,
         std::vector<Sprint> sprints,
         std::string uuid,
         std::vector<Tag> tags,
         bool completed,
         const dw::DateTime& lastModified);

    Task() = default;

    std::string name() const;

    bool isCompleted() const;

    int estimatedCost() const;

    int actualCost() const;

    std::string uuid() const;

    std::vector<Tag> tags() const;

    dw::DateTime lastModified() const;

    GoalProgress goalProgress() const;

    [[nodiscard]] const std::vector<Sprint>& sprints() const;

    void setCompleted(bool completed);

    void addSprint(Sprint sprint);

private:
    std::string taskName;
    int estimated{1};
    std::string id;
    std::vector<Tag> tag;
    bool completed{false};
    dw::DateTime timeStamp{dw::current_date_time_local()};
    std::vector<Sprint> sprintCont;

    bool conflictDetectedWith(const Sprint& sprint) const;
};

std::ostream& operator<<(std::ostream& os, const Task& task);

bool operator==(const Task& lhs, const Task& rhs);

} // namespace sprint_timer

#endif /* end of include guard: TASK_H_7VXCYMOK */
