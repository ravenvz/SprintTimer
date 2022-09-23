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
#ifndef ADDTASKCONTEXT_H_JQ5GOMSB
#define ADDTASKCONTEXT_H_JQ5GOMSB

#include <optional>
#include <string>

namespace sprint_timer::ui {

enum class TaskAddMode { Sibling, Subtask };

class AddTaskContext {
public:
    AddTaskContext();

    AddTaskContext(std::optional<std::string>&& parentUuid,
                   TaskAddMode taskAddMode);

    [[nodiscard]] auto parent() const -> const std::optional<std::string>&;

    [[nodiscard]] auto mode() const -> TaskAddMode;

    friend auto operator==(const AddTaskContext&, const AddTaskContext&)
        -> bool = default;

private:
    std::optional<std::string> parentTaskUuid;
    TaskAddMode taskAddMode{TaskAddMode::Subtask};
};

} // namespace sprint_timer::ui

#endif /* end of include guard: ADDTASKCONTEXT_H_JQ5GOMSB */
