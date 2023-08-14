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
#include "cpp_utils/algorithms/optional_ext.h"
#include <algorithm>
#include <iostream>
#include <utility>

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
           dw::DateTime lastModified_,
           TaskType kind_,
           std::optional<Note> note_,
           TaskTimeframe taskTimeframe_)
    : taskName{std::move(name_)}
    , estimated{estimatedCost_}
    , sprintCont{std::move(sprints_)}
    , id{std::move(uuid_)}
    , tag{std::move(tags_)}
    , completed{completed_}
    , frame{std::move(taskTimeframe_)}
    , note{std::move(note_)}
    , timeStamp{lastModified_}
    , type{kind_}
{
}

auto Task::finish(const std::string& /* uuid */) const -> Task { return *this; }

auto Task::edit(const Task& desiredTask, dw::DateTime currentTime) const -> Task
{
    return Task{desiredTask.name(),
                desiredTask.estimatedCost(),
                sprintCont,
                id,
                desiredTask.tag,
                isCompleted(),
                currentTime,
                desiredTask.kind(),
                desiredTask.notes(),
                desiredTask.timeFrame()};
}

std::string Task::name() const { return taskName; }

bool Task::isCompleted() const { return completed; }

int Task::estimatedCost() const { return estimated; }

int Task::actualCost() const { return static_cast<int>(sprintCont.size()); }

std::string Task::uuid() const { return id; }

auto Task::tags() const -> std::span<const Tag> { return tag; }

DateTime Task::lastModified() const { return timeStamp; }

auto Task::goalProgress() const -> GoalProgress
{
    return GoalProgress{GoalProgress::Estimated{estimatedCost()},
                        GoalProgress::Actual{actualCost()}};
}

auto Task::sprints() const -> std::span<const Sprint> { return sprintCont; }

auto Task::activeSince() const -> dw::DateTime { return frame.start; }

auto Task::dueTo() const -> std::optional<dw::DateTime> { return frame.due; }

auto Task::remindAt() const -> std::optional<dw::DateTime>
{
    return frame.remindAt;
}

auto Task::recurrence() const -> std::optional<Recurrence>
{
    return frame.recurrence;
}

auto Task::notes() const -> std::optional<Note> { return note; }

auto Task::timeFrame() const -> const TaskTimeframe& { return frame; }

auto Task::kind() const -> TaskType { return type; }

auto Task::setCompleted(bool completed_) -> void { completed = completed_; }

auto Task::addSprint(Sprint sprint) -> void
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

auto Task::conflictDetectedWith(const Sprint& sprint) const -> bool
{
    // TODO if sprint order is enforced sorted, might use binary search
    return std::ranges::any_of(sprintCont, sprints_in_conflict{sprint});
}

auto Task::inheritDate(const Task& other, dw::DateTime currentTime) const
    -> Task
{
    if (not other.dueTo()) {
        return *this;
    }
    return Task{
        taskName,
        estimated,
        sprintCont,
        id,
        tag,
        completed,
        currentTime,
        type,
        note,
        TaskTimeframe{other.timeFrame().start, other.timeFrame().due.value()}};
    // TaskTimeframe{
    //     frame.start, other.dueTo(), frame.remindAt, frame.recurrence}};
}

auto Task::inheritDateIfNotSet(const Task& other,
                               dw::DateTime currentTime) const -> Task
{
    if (frame.due or not other.dueTo() or other.frame.due == frame.due) {
        return *this;
    }
    return inheritDate(other, currentTime);
}

auto Task::nextRecurrence(const std::string& nextUuid,
                          dw::DateTime currentTime) const -> std::optional<Task>
{
    auto n_r = [&](const auto& rec) { return rec.nextRecurrence(currentTime); };
    auto n_dt = [&, this](const auto& dt) {
        return Task{
            taskName,
            estimated,
            sprintCont,
            nextUuid,
            tag,
            completed,
            currentTime,
            type,
            note,
            TaskTimeframe{currentTime, dt, frame.remindAt, frame.recurrence}};
    };

    return recurrence().and_then(n_r).transform(n_dt);
}

auto operator<<(std::ostream& os, const Task& task) -> std::ostream&
{
    using alg::inspect;
    os << prefixTags(task.tags());
    if (!task.tags().empty()) {
        os << " ";
    }
    os << task.name() << " ";
    os << task.actualCost() << "/" << task.estimatedCost() << " ";
    os << "Uuid: " << task.uuid() << " ";
    os << "Time: start: " << task.activeSince() << ", due: ";
    if (const auto due = task.dueTo(); due) {
        os << *due;
    }
    else {
        os << "unlimited";
    }
    inspect(task.remindAt(),
            [&](const auto& reminder) { os << ", Reminder: " << reminder; });
    os << ", recurrence: "
       << task.recurrence()
              .transform([](const auto rec) { return rec.pattern(); })
              .value_or("none");
    os << ", modified: " << task.lastModified()
       << ", type: " << std::to_string(static_cast<int>(task.kind()));
    return os;
}

auto operator==(const Task& lhs, const Task& rhs) -> bool
{
    return lhs.uuid() == rhs.uuid() && lhs.name() == rhs.name() &&
           lhs.kind() == rhs.kind() &&
           lhs.estimatedCost() == rhs.estimatedCost() &&
           lhs.actualCost() == rhs.actualCost() &&
           std::equal(cbegin(lhs.tags()),
                      cend(lhs.tags()),
                      cbegin(rhs.tags()),
                      cend(rhs.tags())) &&
           lhs.isCompleted() == rhs.isCompleted() &&
           lhs.notes() == rhs.notes() &&
           lhs.timeFrame() == rhs.timeFrame()
           // There is a reason to compare them by seconds, as last modified
           // timestamp can come from different sources with different precision
           // TODO need to control the sources as this can end up badly
           && dw::to_time_point<std::chrono::seconds>(lhs.lastModified()) ==
                  dw::to_time_point<std::chrono::seconds>(rhs.lastModified());
}

} // namespace sprint_timer
