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
#include "qt_gui/presentation/EditTaskDialogPresenter.h"
#include "cpp_utils/algorithms/optional_ext.h"

namespace sprint_timer::ui {

EditTaskDialogPresenter::EditTaskDialogPresenter(
    edit_task_handler_t& editTaskHandler_,
    all_tags_handler_t& allTagsHandler_,
    const EditTaskContext& editTaskContext_)
    : editTaskHandler{editTaskHandler_}
    , allTagsHandler{allTagsHandler_}
    , editTaskContext{editTaskContext_}
{
}

auto EditTaskDialogPresenter::onEditTaskAccepted(api::TaskDTO&& editedTask)
    -> void
{
    editedTask.uuid = editTaskContext.task().uuid;
    editTaskHandler.handle(api::EditTaskCommand{editedTask});
}

auto EditTaskDialogPresenter::updateViewImpl() -> void
{
    alg::inspect(view(), [&](auto* view) {
        const auto tags = allTagsHandler.handle(api::AllTagsQuery{});
        view->fillTags(tags);
        view->fillTaskDetails(editTaskContext.task());
    });
}

} // namespace sprint_timer::ui

