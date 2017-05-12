/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

#ifndef TASKBUILDER_H_GSWUA7WV
#define TASKBUILDER_H_GSWUA7WV

#include "core/entities/Task.h"
#include "core/utils/OptionalPl.h"

class TaskBuilder {
public:
    Task build();
    TaskBuilder& withName(std::string name);
    TaskBuilder& withEstimatedCost(int cost);
    TaskBuilder& withActualCost(int cost);
    TaskBuilder& withUuid(std::string uuid);
    TaskBuilder& withTag(std::string tag);
    TaskBuilder& withTag(Tag tag);
    TaskBuilder& withLastModificationStamp(DateTime timeStamp);
    TaskBuilder& withExplicitTags(std::list<Tag> tags);
    TaskBuilder& withCompletionStatus(bool completed);
private:
    std::string name;
    int estimatedCost{1};
    int actualCost{0};
    optional<std::string> uuid;
    optional<DateTime> lastModified;
    std::list<Tag> tags;
    bool completionStatus{false};
};


#endif /* end of include guard: TASKBUILDER_H_GSWUA7WV */
