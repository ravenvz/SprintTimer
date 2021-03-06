/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include <optional>

namespace sprint_timer {

class TaskBuilder {
public:
    entities::Task build();
    TaskBuilder& withName(std::string name);
    TaskBuilder& withEstimatedCost(int cost);
    TaskBuilder& withActualCost(int cost);
    TaskBuilder& withUuid(std::string uuid);
    TaskBuilder& withTag(std::string tag);
    TaskBuilder& withTag(entities::Tag tag);
    TaskBuilder& withLastModificationStamp(dw::DateTime timeStamp);
    TaskBuilder& withExplicitTags(std::list<entities::Tag> tags);
    TaskBuilder& withCompletionStatus(bool completed);

private:
    std::string mName;
    int mEstimatedCost{1};
    int mActualCost{0};
    std::optional<std::string> mUuid;
    std::optional<dw::DateTime> mLastModified;
    std::list<entities::Tag> mTags;
    bool mCompletionStatus{false};
};

} // namespace sprint_timer

#endif /* end of include guard: TASKBUILDER_H_GSWUA7WV */
