/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "date_wrapper/DateTime.h"
#include <list>
#include <string>
#include <vector>

namespace sprint_timer::entities {

/* Represent Task that may have many associated sprints.
 *
 * Task has name and tags and might be either in completed or
 * non-completed state. It also has an estimated const in sprints
 * and a number of sprints that were actually spent on this Task. */
class Task {

public:
    Task(std::string name,
         int estimatedCost,
         int actualCost,
         std::list<Tag> tags,
         bool completed,
         const dw::DateTime& lastModified
         = dw::DateTime::currentDateTimeLocal());

    Task(std::string name,
         int estimatedCost,
         int actualCost,
         std::string uuid,
         std::list<Tag> tags,
         bool completed,
         const dw::DateTime& lastModified);

    /* Construct Task from encoded description.
     * Description is a string of text that may have some words with
     * special prefixes. That prefixes indicate tags and number of
     * estimated sprints for this task.
     *
     * Default values are:
     *      '#' - as a tag prefix;
     *      '*' - as an estimated cost prefix.
     *
     * If description has multiple words with tag prefixes, multiple tags
     * will be assigned to the task, but additional rules apply:
     *
     *      only words with single tag prefix are interpreted as tags, so
     *      ##Tag will be interpreted as a part of the name, not as a tag;
     *
     *      single tag prefix (with no characters after it) is also interpreted
     *      as a part of the name.
     *
     * Description can have multiple words with estimated cost prefixes,
     * but only last of them will be interpreted, previous will be interpreted
     * as a part of the name. */
    explicit Task(std::string encodedDescription);

    static std::string estimatedPrefix;

    std::string name() const;

    bool isCompleted() const;

    int estimatedCost() const;

    int actualCost() const;

    std::string uuid() const;

    std::list<Tag> tags() const;

    dw::DateTime lastModified() const;

    void setName(const std::string& name);

    void setCompleted(bool completed);

    void setEstimatedCost(int numSprints);

    void setActualCost(int numSprints);

    void setTags(const std::list<Tag>& newTags);

    void setModifiedTimeStamp(const dw::DateTime& timeStamp);

private:
    std::string name_;
    int estimatedCost_{1};
    int actualCost_{0};
    std::string uuid_;
    std::list<Tag> tags_;
    bool completed_{false};
    dw::DateTime lastModified_;

    void decodeDescription(std::string&& encodedDescription);
};

std::ostream& operator<<(std::ostream& os, const Task& task);

bool operator==(const Task& lhs, const Task& rhs);

} // namespace sprint_timer::entities

#endif /* end of include guard: TASK_H_7VXCYMOK */
