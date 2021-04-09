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
#ifndef TASKCONTRACT_H_MQASHYXA
#define TASKCONTRACT_H_MQASHYXA

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include "qt_gui/presentation/TaskDTO.h"
#include <vector>

namespace sprint_timer::ui::contracts::TaskContract {

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
    virtual void editTask(const TaskDTO& original, const TaskDTO& edited) = 0;

    virtual void deleteTask(const TaskDTO& task) = 0;

    virtual void toggleFinished(const TaskDTO& task) = 0;

    virtual void
    reorderTasks(int32_t sourceRow, int32_t count, int32_t destinationRow) = 0;
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual void displayTasks(const std::vector<TaskDTO>& tasks) = 0;
};

} // namespace sprint_timer::ui::contracts::TaskContract

#endif /* end of include guard: TASKCONTRACT_H_MQASHYXA */
