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
#ifndef EDITTASKDIALOGPRESENTER_H_CRBM0DAY
#define EDITTASKDIALOGPRESENTER_H_CRBM0DAY

#include "api/com_query/CommandHandler.h"
#include "api/com_query/QueryHandler.h"
#include "api/requests/AllTagsQuery.h"
#include "api/requests/EditTaskCommand.h"
#include "api/requests/ReadTaskTreeQuery.h"
#include "qt_gui/presentation/EditTaskContext.h"
#include "qt_gui/presentation/EditTaskDialogContract.h"

namespace sprint_timer::ui {

class EditTaskDialogPresenter
    : public contracts::EditTaskDialogContract::Presenter {
public:
    using edit_task_handler_t = asp::CommandHandler<api::EditTaskCommand>;
    using all_tags_handler_t = asp::QueryHandler<api::AllTagsQuery>;

    EditTaskDialogPresenter(edit_task_handler_t& editTaskHandler_,
                            all_tags_handler_t& allTagsHandler_,
                            const EditTaskContext& editTaskContext_);

    auto onEditTaskAccepted(api::TaskDTO&& editedTask) -> void override;

private:
    edit_task_handler_t& editTaskHandler;
    all_tags_handler_t& allTagsHandler;
    const EditTaskContext& editTaskContext;

    auto updateViewImpl() -> void override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: EDITTASKDIALOGPRESENTER_H_CRBM0DAY */
