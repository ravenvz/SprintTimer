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
#ifndef TASKSELECTIONMEDIATOR_H_K1DXY98M
#define TASKSELECTIONMEDIATOR_H_K1DXY98M

#include "qt_gui/presentation/Mediator.h"
#include "qt_gui/presentation/TaskSelectionContext.h"
#include <string>
#include <vector>

namespace sprint_timer::ui {

class TaskSelectionColleague {
public:
    virtual ~TaskSelectionColleague() = default;

    virtual void onTaskSelectionChanged() = 0;
};

class TaskSelectionMediator : public Mediator<TaskSelectionColleague>,
                              public TaskSelectionContext {
public:
    void changeSelection(TaskSelectionColleague* caller,
                         size_t taskIndex,
                         std::string&& taskUuid)
    {
        index = taskIndex;
        uuid = std::move(taskUuid);

        mediate(caller,
                [](auto* colleague) { colleague->onTaskSelectionChanged(); });
    }

    std::optional<size_t> taskIndex() const override { return index; }

    std::optional<std::string> taskUuid() const override { return uuid; }

private:
    std::optional<size_t> index;
    std::optional<std::string> uuid;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TASKSELECTIONMEDIATOR_H_K1DXY98M */
