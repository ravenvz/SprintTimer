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
#include "api/actions/RegisterSprintBulk.h"
#include <format>
#include <utility>

namespace sprint_timer::api::actions {

RegisterSprintBulk::RegisterSprintBulk(TaskStorage& taskStorage_,
                                       SprintStorage& sprintStorage_,
                                       dw::DateTime timeStamp_,
                                       std::string taskUuid_,
                                       std::vector<Sprint> sprints_)
    : taskStorage{taskStorage_}
    , sprintStorage{sprintStorage_}
    , timeStamp{timeStamp_}
    , taskUuid{std::move(taskUuid_)}
    , sprints{std::move(sprints_)}
{
}

auto RegisterSprintBulk::execute() -> void
{
    auto tasks = taskStorage.findByUuid(taskUuid);
    if (tasks.empty()) {
        throw SprintTimerException{
            std::format("No task with uuid: {}", taskUuid)};
    }
    original = tasks.front();
    Task updated = original.edit(original, timeStamp);
    sprintStorage.save(taskUuid, sprints);
    // updates modificationStamp
    taskStorage.edit(original, updated);
}

auto RegisterSprintBulk::undo() -> void
{
    sprintStorage.remove(sprints);
    // Reverts modificationStamp
    taskStorage.edit(original, original);
}

auto RegisterSprintBulk::describe() const -> std::string
{
    std::stringstream ss;
    ss << "Register new sprint bulk:\n";
    std::copy(sprints.cbegin(),
              sprints.cend(),
              std::ostream_iterator<Sprint>(ss, "\n"));
    std::string res{ss.str()};
    res.pop_back();
    return ss.str();
}

} // namespace sprint_timer::api::actions

