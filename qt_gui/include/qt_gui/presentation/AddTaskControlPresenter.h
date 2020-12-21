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
#ifndef ADDTASKCONTROLPRESENTER_H_KQFERPSW
#define ADDTASKCONTROLPRESENTER_H_KQFERPSW

#include "qt_gui/presentation/AddTaskControl.h"
#include <core/CommandHandler.h>
#include <core/QueryHandler.h>
#include <core/use_cases/create_task/CreateTaskCommand.h>
#include <core/use_cases/request_tags/AllTagsQuery.h>

namespace sprint_timer::ui {

class AddTaskControlPresenter : public contracts::AddTaskControl::Presenter {
public:
    AddTaskControlPresenter(
        CommandHandler<use_cases::CreateTaskCommand>& createTaskHandler,
        QueryHandler<use_cases::AllTagsQuery, std::vector<std::string>>&
            allTagsHandler);

    void onTaskAddConfirmed(
        const contracts::AddTaskControl::TaskDetails& details) const override;

    void
    onTaskAddedInTextForm(const std::string& taskDescription) const override;

    void onDialogRequested() const override;

private:
    CommandHandler<use_cases::CreateTaskCommand>& createTaskHandler;
    QueryHandler<use_cases::AllTagsQuery, std::vector<std::string>>&
        allTagsHandler;

    void updateViewImpl() override;
};

inline AddTaskControlPresenter::AddTaskControlPresenter(
    CommandHandler<use_cases::CreateTaskCommand>& createTaskHandler_,
    QueryHandler<use_cases::AllTagsQuery, std::vector<std::string>>&
        allTagsHandler_)
    : createTaskHandler{createTaskHandler_}
    , allTagsHandler{allTagsHandler_}
{
}

inline void AddTaskControlPresenter::onTaskAddConfirmed(
    const contracts::AddTaskControl::TaskDetails& details) const
{
    std::list<entities::Tag> tags;
    std::transform(cbegin(details.tags),
                   cend(details.tags),
                   std::back_inserter(tags),
                   [](const auto& elem) { return entities::Tag{elem}; });
    createTaskHandler.handle(use_cases::CreateTaskCommand{
        entities::Task{details.name, details.estimatedCost, 0, tags, false}});
}

inline void AddTaskControlPresenter::onTaskAddedInTextForm(
    const std::string& taskDescription) const
{
    const entities::Task task{taskDescription};
    createTaskHandler.handle(use_cases::CreateTaskCommand{task});
}

inline void AddTaskControlPresenter::onDialogRequested() const
{
    auto tags = allTagsHandler.handle(use_cases::AllTagsQuery{});
    view->displayAddTaskDialog(std::move(tags));
}

inline void AddTaskControlPresenter::updateViewImpl() { }

} // namespace sprint_timer::ui

#endif /* end of include guard: ADDTASKCONTROLPRESENTER_H_KQFERPSW */

