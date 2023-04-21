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
#include "api/actions/EditTask.h"

using dw::DateTime;

namespace sprint_timer::api::actions {

EditTask::EditTask(TaskStorage& taskStorage_,
                   Task originalTask_,
                   Task editedTask_)
    : taskStorage{taskStorage_}
    , editedTask{std::move(editedTask_)}
    , originalTask{std::move(originalTask_)}
{
}

auto EditTask::execute() -> void
{
    if (auto due = editedTask.dueTo(); due and originalTask.dueTo() != due) {
        originalSubtree = taskStorage.taskTree().subTree(originalTask.uuid());
        auto updateTimeFrame = [&](const auto& /*uuid*/, const auto& task) {
            if (task.dueTo() and task.dueTo() != due) {
                return;
            }
            const auto t =
                task.inheritDateIfNotSet(editedTask, editedTask.lastModified());
            taskStorage.edit(task, t);
        };
        originalSubtree.dfs(updateTimeFrame);
        editedSubtree = taskStorage.taskTree().subTree(originalTask.uuid());
    }
    taskStorage.edit(originalTask, editedTask);
}

auto EditTask::undo() -> void
{
    for (auto [id, task] : originalSubtree.entriesView()) {
        auto edited = editedSubtree.payload(id);
        if (!edited) {
            throw std::runtime_error{"Keys mismatch!"};
        }
        taskStorage.edit(edited.value().get(), task);
    }
    taskStorage.edit(editedTask, originalTask);
    // for (auto v : std::views::zip(originalSubtree.payloadView(),
    //                               editedSubtree.payloadView())) {
    //     taskStorage.edit(v.first, v.second);
    // }

    // if (subtree) {
    //     subtree->dfs([&](const auto& uuid, const auto& task) {
    //         taskStorage.edit(task, task);
    //     });
    // }
    // else {
    //     taskStorage.edit(editedTask, originalTask);
    // }
}

auto EditTask::describe() const -> std::string
{
    std::stringstream ss;
    ss << "Edit task '" << originalTask << " -> " << editedTask << "'";
    return ss.str();
}

} // namespace sprint_timer::api::actions
