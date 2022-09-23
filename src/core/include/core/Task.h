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
#include "core/Note.h"
#include "core/Sprint.h"
#include "core/Tag.h"
#include "core/TaskTimeframe.h"
#include "core/TaskType.h"
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
    Task(std::string name_,
         int estimatedCost_,
         std::vector<Sprint> sprints_,
         std::string uuid_,
         std::vector<Tag> tags_,
         bool completed_,
         dw::DateTime lastModified_,
         TaskType kind_,
         std::optional<Note> note_,
         TaskTimeframe taskTimeframe_);

    Task() = default;

    // Task(Task&& other) = default;
    //
    // Task& operator=(Task&& other) = default;

    [[nodiscard]] auto finish() const -> Task;

    [[nodiscard]] auto edit(const Task& desiredTask) const -> Task;

    [[nodiscard]] auto name() const -> std::string;

    [[nodiscard]] auto isCompleted() const -> bool;

    [[nodiscard]] auto estimatedCost() const -> int;

    [[nodiscard]] auto actualCost() const -> int;

    [[nodiscard]] auto uuid() const -> std::string;

    [[nodiscard]] auto tags() const -> std::span<const Tag>;

    [[nodiscard]] auto lastModified() const -> dw::DateTime;

    [[nodiscard]] auto goalProgress() const -> GoalProgress;

    [[nodiscard]] auto sprints() const -> std::span<const Sprint>;

    [[nodiscard]] auto activeSince() const -> dw::DateTime;

    [[nodiscard]] auto dueTo() const -> std::optional<dw::DateTime>;

    [[nodiscard]] auto remindAt() const -> std::optional<dw::DateTime>;

    [[nodiscard]] auto recurrence() const -> std::optional<Recurrence>;

    [[nodiscard]] auto notes() const -> std::optional<Note>;

    [[nodiscard]] auto timeFrame() const -> const TaskTimeframe&;

    [[nodiscard]] auto kind() const -> TaskType;

    auto setCompleted(bool completed) -> void;

    auto addSprint(Sprint sprint) -> void;

private:
    std::string taskName;
    int estimated{0};
    std::vector<Sprint> sprintCont;
    std::string id;
    std::vector<Tag> tag;
    bool completed{false};
    TaskTimeframe frame;
    std::optional<Note> note;
    dw::DateTime timeStamp{dw::current_date_time_local()};
    TaskType type{TaskType::Regular};

    [[nodiscard]] auto conflictDetectedWith(const Sprint& sprint) const -> bool;
};

auto operator<<(std::ostream& os, const Task& task) -> std::ostream&;

auto operator==(const Task& lhs, const Task& rhs) -> bool;

} // namespace sprint_timer

#endif /* end of include guard: TASK_H_7VXCYMOK */
