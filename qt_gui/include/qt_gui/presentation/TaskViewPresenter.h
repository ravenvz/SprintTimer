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

#include "qt_gui/presentation/TaskSelectionMediator.h"
#include "qt_gui/presentation/TaskViewContract.h"

namespace sprint_timer::ui {

class TaskViewPresenter : public contracts::TaskViewContract::Presenter,
                          public TaskSelectionColleague {
public:
    explicit TaskViewPresenter(TaskSelectionMediator& taskSelectionMediator);

    ~TaskViewPresenter() override;

    void changeTaskSelection(size_t index, std::string&& uuid) override;

    void onTaskSelectionChanged() override;

private:
    TaskSelectionMediator& taskSelectionMediator;

    void updateViewImpl() override;

    void onViewAttached() override;
};

inline TaskViewPresenter::TaskViewPresenter(
    TaskSelectionMediator& taskSelectionMediator_)
    : taskSelectionMediator{taskSelectionMediator_}
{
    taskSelectionMediator.addColleague(this);
}

inline TaskViewPresenter::~TaskViewPresenter()
{
    taskSelectionMediator.removeColleague(this);
}

inline void TaskViewPresenter::changeTaskSelection(size_t index,
                                                   std::string&& uuid)
{
    taskSelectionMediator.changeSelection(this, index, std::move(uuid));
}

inline void TaskViewPresenter::updateViewImpl() { onTaskSelectionChanged(); }

inline void TaskViewPresenter::onViewAttached() { updateView(); }

inline void TaskViewPresenter::onTaskSelectionChanged()
{
    if (auto v = view(); v) {
        v.value()->selectTask(taskSelectionMediator.taskIndex());
    }
}

} // namespace sprint_timer::ui

#endif /* end of include guard: UNFINISHEDTASKSPRESENTER_H_LUJVQIYM */
