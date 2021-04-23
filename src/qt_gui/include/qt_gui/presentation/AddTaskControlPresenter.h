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
#ifndef ADDTASKCONTROLPRESENTER_H_KQFERPSW
#define ADDTASKCONTROLPRESENTER_H_KQFERPSW

#include "core/CommandHandler.h"
#include "core/QueryHandler.h"
#include "core/use_cases/create_task/CreateTaskCommand.h"
#include "core/use_cases/request_tags/AllTagsQuery.h"
#include "qt_gui/presentation/AddTaskControl.h"
#include "core/use_cases/TaskMapper.h"

namespace sprint_timer::ui {

class AddTaskControlPresenter : public contracts::AddTaskControl::Presenter {
public:
    explicit AddTaskControlPresenter(
        CommandHandler<use_cases::CreateTaskCommand>& createTaskHandler);

    void addTask(const use_cases::TaskDTO& details) const override;

    void addTask(const std::string& encodedDescription) const override;

private:
    CommandHandler<use_cases::CreateTaskCommand>& createTaskHandler;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: ADDTASKCONTROLPRESENTER_H_KQFERPSW */

