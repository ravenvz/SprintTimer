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
#include "api/DateTimeProvider.h"
#include "api/SprintStorage.h"
#include "api/TaskStorage.h"

namespace sprint_timer::api::actions {

class RegisterSprintBulk {
public:
    RegisterSprintBulk(TaskStorage& taskStorage_,
                       SprintStorage& sprintStorage_,
                       dw::DateTime timeStamp_,
                       std::string taskUuid_,
                       std::vector<Sprint> sprints_);

    auto execute() -> void;

    auto undo() -> void;

    [[nodiscard]] auto describe() const -> std::string;

private:
    TaskStorage& taskStorage;
    SprintStorage& sprintStorage;
    dw::DateTime timeStamp;
    std::string taskUuid;
    std::vector<Sprint> sprints;
    Task original;
};

} // namespace sprint_timer::api::actions
