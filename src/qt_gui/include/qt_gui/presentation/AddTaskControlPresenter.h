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
#ifndef ADDTASKCONTROLPRESENTER_H_KQFERPSW
#define ADDTASKCONTROLPRESENTER_H_KQFERPSW

#include "qt_gui/presentation/AddTaskControl.h"
#include "qt_gui/presentation/TaskMapper.h"
#include "core/CommandHandler.h"
#include "core/QueryHandler.h"
#include "core/use_cases/create_task/CreateTaskCommand.h"
#include "core/use_cases/request_tags/AllTagsQuery.h"

namespace sprint_timer::ui {

class AddTaskControlPresenter : public contracts::AddTaskControl::Presenter {
public:
    explicit AddTaskControlPresenter(
        CommandHandler<use_cases::CreateTaskCommand>& createTaskHandler);

    void addTask(const TaskDTO& details) const override;

    void addTask(const std::string& encodedDescription) const override;

private:
    CommandHandler<use_cases::CreateTaskCommand>& createTaskHandler;
};

inline AddTaskControlPresenter::AddTaskControlPresenter(
    CommandHandler<use_cases::CreateTaskCommand>& createTaskHandler_)
    : createTaskHandler{createTaskHandler_}
{
}

inline void AddTaskControlPresenter::addTask(const TaskDTO& details) const
{
    // TODO Remove task creation when CreateTaskCommand is simplified
    std::list<entities::Tag> tags(details.tags.size());
    std::transform(cbegin(details.tags),
                   cend(details.tags),
                   begin(tags),
                   [](const auto& elem) { return entities::Tag{elem}; });
    entities::Task task{details.name,
                        details.expectedCost,
                        0,
                        tags,
                        details.finished,
                        details.modificationStamp};

    createTaskHandler.handle(use_cases::CreateTaskCommand{task});
    // createTaskHandler.handle(use_cases::CreateTaskCommand{fromDTO(details)});
}

inline void
AddTaskControlPresenter::addTask(const std::string& encodedDescription) const
{
    const entities::Task task{encodedDescription};
    createTaskHandler.handle(use_cases::CreateTaskCommand{task});
}

} // namespace sprint_timer::ui

#endif /* end of include guard: ADDTASKCONTROLPRESENTER_H_KQFERPSW */

