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
#include "qt_gui/presentation/AddTaskDialogPresenter.h"
#include "core/TaskTree.h"

namespace {

auto addSiblingBelow(
    sprint_timer::ui::contracts::AddTaskDialogContract::View* view,
    const sprint_timer::api::TaskTreeDTO& taskTree,
    const sprint_timer::ui::AddTaskContext& context) -> void;

auto addSubtaskLast(
    sprint_timer::ui::contracts::AddTaskDialogContract::View* view,
    const sprint_timer::api::TaskTreeDTO& taskTree,
    const sprint_timer::ui::AddTaskContext& context) -> void;

auto makeParentSearchStrategy(sprint_timer::ui::TaskAddMode mode)
{
    switch (mode) {
    case sprint_timer::ui::TaskAddMode::Subtask:
        return addSubtaskLast;
    case sprint_timer::ui::TaskAddMode::Sibling:
        return addSiblingBelow;
    }
    return addSubtaskLast;
}

} // namespace

namespace sprint_timer::ui {

AddTaskDialogPresenter::AddTaskDialogPresenter(
    create_task_handler_t& createTaskHandler_,
    all_tags_handler_t& allTagsHandler_,
    read_task_tree_handler_t& readTaskTreeHandler_,
    const AddTaskContext& addTaskContext_)
    : createTaskHandler{createTaskHandler_}
    , allTagsHandler{allTagsHandler_}
    , readTaskTreeHandler{readTaskTreeHandler_}
    , addTaskContext{addTaskContext_}
{
}

auto AddTaskDialogPresenter::onTaskCreationAccepted(
    std::string&& name,
    std::vector<std::string>&& tags,
    int32_t estimatedCost,
    api::TaskTypeDTO taskType,
    std::optional<std::string>&& parent,
    std::optional<int64_t> beforePosition,
    std::optional<api::NoteDTO>&& notes,
    api::TaskTimeframeDTO&& taskFrame) -> void
{
    createTaskHandler.handle(api::CreateTaskCommand{std::move(name),
                                                    std::move(tags),
                                                    estimatedCost,
                                                    taskType,
                                                    std::move(parent),
                                                    beforePosition,
                                                    std::move(notes),
                                                    std::move(taskFrame)});
}

auto AddTaskDialogPresenter::updateViewImpl() -> void
{
    alg::inspect(view(), [&](auto* view) {
        const auto tags = allTagsHandler.handle(api::AllTagsQuery{});
        view->fillTags(tags);
        const auto taskTree =
            readTaskTreeHandler.handle(api::ReadTaskTreeQuery{});
        const auto mode = addTaskContext.mode();
        makeParentSearchStrategy(mode)(view, taskTree, addTaskContext);
    });
}

} // namespace sprint_timer::ui

namespace {

auto addSiblingBelow(
    sprint_timer::ui::contracts::AddTaskDialogContract::View* view,
    const sprint_timer::api::TaskTreeDTO& taskTree,
    const sprint_timer::ui::AddTaskContext& context) -> void
{
    alg::inspect(context.parent(), [&](const auto& siblingUuid) {
        auto it = std::ranges::find(
            taskTree, siblingUuid, &sprint_timer::api::TaskDTO::uuid);
        const auto position = taskTree.position_in_children(it) + 1;
        auto parent_it = taskTree.parent(it);
        std::optional<std::string> parent_uuid{
            parent_it == taskTree.cend() ? std::optional<std::string>{}
                                         : parent_it->uuid};
        // const auto parent = taskTree.parent(siblingUuid);
        // const auto position =
        //     taskTree.positionInChildren(siblingUuid).transform([&](auto pos)
        //     {
        //         return static_cast<int64_t>(pos) + 1;
        //     });
        view->fillParentData(parent_uuid, position);
    });
}

auto addSubtaskLast(
    sprint_timer::ui::contracts::AddTaskDialogContract::View* view,
    const sprint_timer::api::TaskTreeDTO& taskTree,
    const sprint_timer::ui::AddTaskContext& context) -> void
{
    const auto childrenSize =
        context.parent()
            .transform([&](const auto& uuid) {
                auto it = std::ranges::find(
                    taskTree, uuid, &sprint_timer::api::TaskDTO::uuid);
                return taskTree.children(it).size();
            })
            .value_or(taskTree.children(taskTree.cend()).size());
    view->fillParentData(context.parent(), static_cast<int64_t>(childrenSize));
}

} // namespace
