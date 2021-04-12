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
#ifndef TASKSPRINTSPRESENTER_H_VN5LZSTF
#define TASKSPRINTSPRESENTER_H_VN5LZSTF

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/presentation/SprintMapper.h"
#include "qt_gui/presentation/TaskSelectionContext.h"
#include "qt_gui/presentation/TaskSprintsContract.h"
#include "core/QueryHandler.h"
#include "core/use_cases/request_sprints/SprintsForTaskQuery.h"

namespace sprint_timer::ui {

class TaskSprintsPresenter
    : public mvp::BasePresenter<contracts::TaskSprintsContract::View> {
public:
    using sprints_for_task_hdl_t = QueryHandler<use_cases::SprintsForTaskQuery,
                                                std::vector<entities::Sprint>>;

    TaskSprintsPresenter(sprints_for_task_hdl_t& sprintsForTaskHandler,
                         const TaskSelectionContext& taskSelectionContext);

private:
    sprints_for_task_hdl_t& sprintsForTaskHandler;
    const TaskSelectionContext& taskSelectionContext;

    void updateViewImpl() override;

    void onViewAttached() override;
};

inline TaskSprintsPresenter::TaskSprintsPresenter(
    sprints_for_task_hdl_t& sprintsForTaskHandler_,
    const TaskSelectionContext& taskSelectionContext_)
    : sprintsForTaskHandler{sprintsForTaskHandler_}
    , taskSelectionContext{taskSelectionContext_}
{
}

inline void TaskSprintsPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        if (auto uuid = taskSelectionContext.taskUuid(); uuid) {
            const auto sprints = sprintsForTaskHandler.handle(
                use_cases::SprintsForTaskQuery{std::move(*uuid)});
            v.value()->displaySprints(makeDTOs(sprints));
        }
    }
}

inline void TaskSprintsPresenter::onViewAttached() { updateView(); }

} // namespace sprint_timer::ui

#endif /* end of include guard: TASKSPRINTSPRESENTER_H_VN5LZSTF */
