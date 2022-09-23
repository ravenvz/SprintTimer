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
#ifndef PLANNERPRESENTER_H_FPLV5C7S
#define PLANNERPRESENTER_H_FPLV5C7S

#include "api/DateTimeProvider.h"
#include "api/com_query/CommandHandler.h"
#include "api/com_query/QueryHandler.h"
#include "api/dtos/TaskTreeDTO.h"
#include "api/requests/CreateTaskCommand.h"
#include "api/requests/DeleteTaskCommand.h"
#include "api/requests/EditTaskCommand.h"
#include "api/requests/ReadTaskTreeQuery.h"
#include "api/requests/SaveTaskTreeCommand.h"
#include "qt_gui/presentation/AddTaskContext.h"
#include "qt_gui/presentation/EditTaskContext.h"
#include "qt_gui/presentation/PlannerContract.h"

namespace sprint_timer::ui {

struct PlannerColors {
    std::string_view text;
    std::string_view contrastText;
    std::string_view defaultBackround;
    std::string_view normalWork;
    std::string_view doneWork;
    std::string_view overworkWork;
    std::string_view dueNotSoon;
    std::string_view dueSoon;
    std::string_view dueToday;
    std::string_view dueOverdue;
    std::string_view tag;
};

class PlannerPresenter : public contracts::PlannerContract::Presenter {
public:
    using read_planner_handler_t = asp::QueryHandler<api::ReadTaskTreeQuery>;
    using save_planner_handler_t = asp::QueryHandler<api::SaveTaskTreeCommand>;
    using delete_task_handler_t = asp::CommandHandler<api::DeleteTaskCommand>;

    PlannerPresenter(PlannerColors colors_,
                     read_planner_handler_t& readPlannerHandler_,
                     save_planner_handler_t& savePlannerHandler_,
                     delete_task_handler_t& deleteTaskHandler_,
                     AddTaskContext& addTaskContext_,
                     EditTaskContext& editTaskContext_,
                     const api::DateTimeProvider& timeProvider_);

    auto moveNodes(const std::optional<std::string>& sourceParent,
                   int64_t sourceRow,
                   int64_t count,
                   const std::optional<std::string>& destinationParent,
                   int64_t destinationChild) -> void override;

    auto deleteTask(std::string&& taskUuid) -> void override;

    auto changeTaskAdditionContext(std::optional<std::string>&& parent,
                                   bool isSubtask) -> void override;

    auto changeTaskEditionContext(const std::string& uuid) -> void override;

private:
    PlannerColors colors;
    read_planner_handler_t& readPlannerHandler;
    save_planner_handler_t& savePlannerHandler;
    delete_task_handler_t& deleteTaskHandler;
    AddTaskContext& addTaskContext;
    EditTaskContext& editTaskContext;
    const api::DateTimeProvider& timeProvider;
    api::TaskTreeDTO data;

    void updateViewImpl() override;

    void fetchDataImpl() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: PLANNERPRESENTER_H_FPLV5C7S */
