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

#include "core/entities/Sprint.h"
#include "core/entities/Tag.h"
#include "core/utils/StringUtils.h"
#include "date_wrapper/date_wrapper.h"
#include <list>

namespace sprint_timer::entities {

/* Represent Task that may have many associated sprints.
 *
 * Task has name and tags and might be either in completed or
 * non-completed state. It also has an estimated const in sprints
 * and a number of sprints that were actually spent on this Task. */
class Task {

public:
    Task(std::string uuid,
         std::string name,
         int estimatedCost,
         dw::DateTime lastModified);

    Task(std::string name,
         int estimatedCost,
         std::vector<Sprint> sprints,
         std::string uuid,
         std::list<Tag> tags,
         bool completed,
         const dw::DateTime& lastModified);

    Task(std::string name,
         int estimatedCost,
         int actualCost,
         std::string uuid,
         std::list<Tag> tags,
         bool completed,
         const dw::DateTime& lastModified);

    Task() = default;

    std::string name() const;

    bool isCompleted() const;

    int estimatedCost() const;

    int actualCost() const;

    std::string uuid() const;

    std::list<Tag> tags() const;

    dw::DateTime lastModified() const;

    const std::vector<Sprint>& sprints() const;

    void setName(const std::string& name);

    void setCompleted(bool completed);

    void setEstimatedCost(int numSprints);

    void setActualCost(int numSprints);

    void setTags(const std::list<Tag>& newTags);

    void setModifiedTimeStamp(const dw::DateTime& timeStamp);

    void addSprint(Sprint sprint);

private:
    std::string taskName;
    int estimated{1};
    int actual{0};
    std::string id;
    std::list<Tag> tag;
    bool completed{false};
    dw::DateTime timeStamp{dw::current_date_time_local()};
    std::vector<Sprint> sprintCont;

    bool conflictDetectedWith(const Sprint& sprint) const;
};

std::ostream& operator<<(std::ostream& os, const Task& task);

bool operator==(const Task& lhs, const Task& rhs);

} // namespace sprint_timer::entities

#endif /* end of include guard: TASK_H_7VXCYMOK */
