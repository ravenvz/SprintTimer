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
#ifndef UNFINISHEDTASKSCONTRACT_H_3TEOUIV4
#define UNFINISHEDTASKSCONTRACT_H_3TEOUIV4

#include "qt_gui/presentation/BasePresenter.h"
#include <core/entities/Task.h>

namespace sprint_timer::ui::contracts::UnfinishedTasksContract {

class View {
public:
    virtual void displayTasks(const std::vector<entities::Task>& tasks) = 0;

    virtual void selectTask(size_t taskIndex) = 0;

    virtual ~View() = default;
    View() = default;
    View(const View& other) = delete;
    View& operator=(const View& other) = delete;
    View(View&& other) noexcept = delete;
    View& operator=(View&& other) noexcept = delete;
};

class Presenter : public BasePresenter<View> {
public:
    virtual void onTaskDelete(const entities::Task& task) = 0;

    virtual void onViewAllTaskSprints(const std::string& taskUuid) = 0;

    virtual void onTaskEdit(entities::Task&& oldTask,
                            entities::Task&& updatedTask) = 0;

    virtual void onToggleTaskComplete(const std::string& taskUuid) = 0;

    virtual void onTagEditorRequested() = 0;

    virtual void onSprintsForTaskRequested() = 0;

    virtual void
    onQuickAddTaskStringEntered(const std::string& encodedTask) = 0;

    virtual void onAddTaskClicked() = 0;

    virtual void onTaskSelectionChanged(View& viewWhereSelectionChanged,
                                        size_t selectionIndex) = 0;

    virtual void
    onRegisterSprints(size_t taskIndex,
                      const std::vector<dw::DateTimeRange>& timeRanges) = 0;
};

} // namespace sprint_timer::ui::contracts::UnfinishedTasksContract

#endif /* end of include guard: UNFINISHEDTASKSCONTRACT_H_3TEOUIV4 */
