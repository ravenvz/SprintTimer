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
#ifndef ADDTASKDIALOGPRESENTER_H_728SJBXR
#define ADDTASKDIALOGPRESENTER_H_728SJBXR

#include "api/com_query/CommandHandler.h"
#include "api/com_query/QueryHandler.h"
#include "api/requests/AllTagsQuery.h"
#include "api/requests/ReadTaskTreeQuery.h"
#include "qt_gui/presentation/AddTaskContext.h"
#include "qt_gui/presentation/AddTaskDialogContract.h"

namespace sprint_timer::ui {

class AddTaskDialogPresenter
    : public contracts::AddTaskDialogContract::Presenter {
public:
    using create_task_handler_t = asp::CommandHandler<api::CreateTaskCommand>;
    using all_tags_handler_t = asp::QueryHandler<api::AllTagsQuery>;
    using read_task_tree_handler_t = asp::QueryHandler<api::ReadTaskTreeQuery>;

    AddTaskDialogPresenter(create_task_handler_t& createTaskHandler,
                           all_tags_handler_t& allTagsHandler,
                           read_task_tree_handler_t& readTaskTreeHandler,
                           const AddTaskContext& addTaskContext);

    auto onTaskCreationAccepted(std::string&& name,
                                std::vector<std::string>&& tags,
                                int32_t estimatedCost,
                                api::TaskTypeDTO taskType,
                                std::optional<std::string>&& parent,
                                std::optional<int64_t> beforePosition,
                                std::optional<api::NoteDTO>&& notes,
                                api::TaskTimeframeDTO&& taskFrame)
        -> void override;

private:
    create_task_handler_t& createTaskHandler;
    all_tags_handler_t& allTagsHandler;
    read_task_tree_handler_t& readTaskTreeHandler;
    const AddTaskContext& addTaskContext;

    auto updateViewImpl() -> void override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: ADDTASKDIALOGPRESENTER_H_728SJBXR */
