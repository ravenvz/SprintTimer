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
#ifndef UNFINISHEDTASKSPRESENTER_H_LUJVQIYM
#define UNFINISHEDTASKSPRESENTER_H_LUJVQIYM

#include "qt_gui/presentation/UnfinishedTasksContract.h"
#include <core/CommandHandler.h>
#include <core/QueryHandler.h>
#include <core/use_cases/delete_task/DeleteTaskCommand.h>
#include <core/use_cases/edit_task/EditTaskCommand.h>
#include <core/use_cases/register_sprint/RegisterSprintBulkCommand.h>
#include <core/use_cases/request_tasks/UnfinishedTasksQuery.h>

namespace sprint_timer::ui {

class UnfinishedTasksPresenter
    : public contracts::UnfinishedTasksContract::Presenter {
public:
    UnfinishedTasksPresenter(
        QueryHandler<use_cases::UnfinishedTasksQuery,
                     std::vector<entities::Task>>& unfinishedTasksHandler,
        CommandHandler<use_cases::DeleteTaskCommand>& deleteTaskHandler,
        CommandHandler<use_cases::EditTaskCommand>& editTaskHandler,
        CommandHandler<use_cases::RegisterSprintBulkCommand>&
            registerSprintBulkHandler);

    void attachView(contracts::UnfinishedTasksContract::View& view) override;

    void onTaskDelete(const entities::Task& task) override;

    void onViewAllTaskSprints(const std::string& taskUuid) override;

    void onTaskEdit(entities::Task&& oldTask,
                    entities::Task&& updatedTask) override;

    void onToggleTaskComplete(const std::string& taskUuid) override;

    void onTagEditorRequested() override;

    void onSprintsForTaskRequested() override;

    void onQuickAddTaskStringEntered(const std::string& encodedTask) override;

    void onAddTaskClicked() override;

    void onTaskSelectionChanged(
        contracts::UnfinishedTasksContract::View& viewWhereSelectionChanged,
        size_t selectionIndex) override;

    void updateViewImpl() override;

    void onRegisterSprints(
        size_t taskIndex,
        const std::vector<dw::DateTimeRange>& timeRanges) override;

private:
    QueryHandler<use_cases::UnfinishedTasksQuery, std::vector<entities::Task>>&
        unfinishedTasksHandler;
    CommandHandler<use_cases::DeleteTaskCommand>& deleteTaskHandler;
    CommandHandler<use_cases::EditTaskCommand>& editTaskHandler;
    CommandHandler<use_cases::RegisterSprintBulkCommand>&
        registerSprintBulkHandler;
    std::vector<
        std::reference_wrapper<contracts::UnfinishedTasksContract::View>>
        views;
    std::vector<entities::Task> unfinishedTasks;
};

inline UnfinishedTasksPresenter::UnfinishedTasksPresenter(
    QueryHandler<use_cases::UnfinishedTasksQuery, std::vector<entities::Task>>&
        unfinishedTasksHandler_,
    CommandHandler<use_cases::DeleteTaskCommand>& deleteTaskHandler_,
    CommandHandler<use_cases::EditTaskCommand>& editTaskHandler_,
    CommandHandler<use_cases::RegisterSprintBulkCommand>&
        registerSprintBulkHandler_)
    : unfinishedTasksHandler{unfinishedTasksHandler_}
    , deleteTaskHandler{deleteTaskHandler_}
    , editTaskHandler{editTaskHandler_}
    , registerSprintBulkHandler{registerSprintBulkHandler_}
{
    unfinishedTasks =
        unfinishedTasksHandler.handle(use_cases::UnfinishedTasksQuery{});
}

inline void UnfinishedTasksPresenter::attachView(
    contracts::UnfinishedTasksContract::View& view_)
{
    views.emplace_back(view_);
    view_.displayTasks(unfinishedTasks);
}

inline void UnfinishedTasksPresenter::onTaskDelete(const entities::Task& task)
{
    deleteTaskHandler.handle(use_cases::DeleteTaskCommand{task});
}

inline void
UnfinishedTasksPresenter::onViewAllTaskSprints(const std::string& taskUuid)
{
}

inline void UnfinishedTasksPresenter::onTaskEdit(entities::Task&& oldTask,
                                                 entities::Task&& updatedTask)
{
    editTaskHandler.handle(
        use_cases::EditTaskCommand{std::move(oldTask), std::move(updatedTask)});
}

inline void
UnfinishedTasksPresenter::onToggleTaskComplete(const std::string& taskUuid)
{
}

inline void UnfinishedTasksPresenter::onTagEditorRequested() { }

inline void UnfinishedTasksPresenter::onSprintsForTaskRequested() { }

inline void UnfinishedTasksPresenter::onQuickAddTaskStringEntered(
    const std::string& encodedTask)
{
}

inline void UnfinishedTasksPresenter::onAddTaskClicked() { }

inline void UnfinishedTasksPresenter::onTaskSelectionChanged(
    contracts::UnfinishedTasksContract::View& viewWhereSelectionChanged,
    size_t selectionIndex)
{
    for (const auto& v : views) {
        if (&v.get() == &viewWhereSelectionChanged) {
            continue;
        }
        v.get().selectTask(selectionIndex);
    }
}

inline void UnfinishedTasksPresenter::updateViewImpl() { }

inline void UnfinishedTasksPresenter::onRegisterSprints(
    size_t taskIndex, const std::vector<dw::DateTimeRange>& timeRanges)
{
    std::vector<entities::Sprint> sprints;
    sprints.reserve(timeRanges.size());
    const auto& task = unfinishedTasks[taskIndex];
    std::transform(cbegin(timeRanges),
                   cend(timeRanges),
                   std::back_inserter(sprints),
                   [&](const auto& elem) {
                       return entities::Sprint{
                           task.name(), elem, task.tags(), task.uuid()};
                   });
    registerSprintBulkHandler.handle(
        use_cases::RegisterSprintBulkCommand{std::move(sprints)});
}

} // namespace sprint_timer::ui

#endif /* end of include guard: UNFINISHEDTASKSPRESENTER_H_LUJVQIYM */
