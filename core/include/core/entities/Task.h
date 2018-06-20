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


#include "core/BoostUUIDGenerator.h"
#include "core/StringUtils.h"
#include "core/entities/Sprint.h"
#include "core/entities/Tag.h"
#include "date_wrapper/DateTime.h"
#include <list>
#include <string>
#include <vector>

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

    /* Return Task name. */
    std::string name() const;

    /* Return true if Task is completed and false otherwise. */
    bool isCompleted() const;

    /* Return estimated number of sprints that are required to completed this
     * task. */
    int estimatedCost() const;

    /* Number of sprints that had been spent on this task. */
    int actualCost() const;

    /* Return UUID. */
    std::string uuid() const;

    /* Return list containing Task tags. */
    std::list<Tag> tags() const;

    dw::DateTime lastModified() const;

    void setName(const std::string& name);

    /* Set task status. */
    void setCompleted(bool completed);

    /* Set estimated number of sprints that are required to complete this task.
     */
    void setEstimatedCost(int numSprints);

    /* Set number of sprints spent on this task. */
    void setActualCost(int numSprints);

    void setTags(const std::list<Tag>& newTags);

    /* Set time stamp of last item modification. */
    void setModifiedTimeStamp(const dw::DateTime& timeStamp);

    friend std::ostream& operator<<(std::ostream& os, const Task& task);

private:
    static BoostUUIDGenerator generator;
    std::string mName;
    int mEstimatedCost{1};
    int mActualCost{0};
    std::string mUuid;
    std::list<Tag> mTags;
    bool mCompleted{false};
    dw::DateTime mLastModified;

    void decodeDescription(std::string&& encodedDescription);
};

bool operator==(const Task& lhs, const Task& rhs);

#endif /* end of include guard: TASK_H_7VXCYMOK */
