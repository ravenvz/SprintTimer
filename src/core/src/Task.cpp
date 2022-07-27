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
#include "core/Task.h"
#include "core/SprintTimerException.h"
#include <algorithm>
#include <iostream>

namespace {

struct sprints_in_conflict {
    sprints_in_conflict(const sprint_timer::Sprint& sprint_)
        : sprint{sprint_}
    {
    }

    bool operator()(const sprint_timer::Sprint& otherSprint) const
    {
        return sprint_timer::intersectingInTime(sprint, otherSprint);
    }

private:
    const sprint_timer::Sprint& sprint;
};

} // namespace

namespace sprint_timer {

using dw::DateTime;

Task::Task(std::string name_,
           int estimatedCost_,
           std::vector<Sprint> sprints_,
           std::string uuid_,
           std::vector<Tag> tags_,
           bool completed_,
           const dw::DateTime& lastModified_)
    : taskName{std::move(name_)}
    , estimated{estimatedCost_}
    , id{std::move(uuid_)}
    , tag{std::move(tags_)}
    , completed{completed_}
    , timeStamp{lastModified_}
    , sprintCont{std::move(sprints_)}
{
}

std::string Task::name() const { return taskName; }

bool Task::isCompleted() const { return completed; }

int Task::estimatedCost() const { return estimated; }

int Task::actualCost() const { return static_cast<int>(sprintCont.size()); }

std::string Task::uuid() const { return id; }

std::vector<Tag> Task::tags() const { return tag; }

DateTime Task::lastModified() const { return timeStamp; }

auto Task::goalProgress() const -> GoalProgress
{
    return GoalProgress{GoalProgress::Estimated{estimatedCost()},
                        GoalProgress::Actual{actualCost()}};
}

auto Task::sprints() const -> const std::vector<Sprint>& { return sprintCont; }

void Task::setCompleted(bool completed_) { completed = completed_; }

void Task::addSprint(Sprint sprint)
{
    if (conflictDetectedWith(sprint)) {
        std::stringstream ss;
        ss << "Attempting to add sprint that conflicts with others:\n";
        ss << sprint;
        ss << "\nin conflict with:\n";
        for (const auto& s : sprintCont) {
            if (sprints_in_conflict{sprint}(s)) {
                ss << s << '\n';
            }
        }
        throw SprintTimerException{ss.str()};
    }
    sprintCont.push_back(std::move(sprint));
    timeStamp = dw::current_date_time_local();
}

bool Task::conflictDetectedWith(const Sprint& sprint) const
{
    // TODO if sprint order is enforced sorted, might use binary search
    return std::ranges::any_of(sprintCont, sprints_in_conflict{sprint});
}

std::ostream& operator<<(std::ostream& os, const Task& task)
{
    os << prefixTags(task.tags());
    if (!task.tags().empty())
        os << " ";
    os << task.name() << " ";
    os << task.actualCost() << "/" << task.estimatedCost() << " ";
    os << "Uuid: " << task.uuid() << " ";
    os << task.lastModified();
    return os;
}

bool operator==(const Task& lhs, const Task& rhs)
{
    return lhs.uuid() == rhs.uuid() && lhs.name() == rhs.name() &&
           lhs.estimatedCost() == rhs.estimatedCost() &&
           lhs.actualCost() == rhs.actualCost() && lhs.tags() == rhs.tags() &&
           lhs.isCompleted() == rhs.isCompleted()
           // There is a reason to compare them by seconds, as last modified
           // timestamp can come from different sources with different precision
           // TODO need to control the sources as this can end up badly
           && dw::to_time_point<std::chrono::seconds>(lhs.lastModified()) ==
                  dw::to_time_point<std::chrono::seconds>(rhs.lastModified());
}

} // namespace sprint_timer
