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
#ifndef ACTIVETASKSCONTRACT_H_6FEWWZAP
#define ACTIVETASKSCONTRACT_H_6FEWWZAP

#include <core/entities/Task.h>

namespace sprint_timer::ui::contracts::ActiveTasksContract {

class View {
public:
    virtual void displayTasks(const std::vector<entities::Task>& tasks) = 0;

    virtual ~View() = default;
    View(const View& other) = delete;
    View& operator=(const View& other) = delete;
    View(View&& other) noexcept = delete;
    View& operator=(View&& other) noexcept = delete;
};

class Presenter {
public:
    virtual void onTaskDelete() = 0;

    virtual ~Presenter() = default;
    Presenter(const Presenter& other) = delete;
    Presenter& operator=(const Presenter& other) = delete;
    Presenter(Presenter&& other) noexcept = delete;
    Presenter& operator=(Presenter&& other) noexcept = delete;
};

} // namespace sprint_timer::ui::contracts::ActiveTasksContract

#endif /* end of include guard: ACTIVETASKSCONTRACT_H_6FEWWZAP */
