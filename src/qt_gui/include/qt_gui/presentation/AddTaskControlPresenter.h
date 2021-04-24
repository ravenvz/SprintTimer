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

    /* Construct Task from encoded description.
     * Description is a string of text that may have some words with
     * special prefixes. That prefixes indicate tags and number of
     * estimated sprints for this task.
     *
     * Default values are:
     *      '#' - as a tag prefix;
     *      '*' - as an estimated cost prefix.
     *
     * If description has multiple words with tag prefixes, multiple tags
     * will be assigned to the task, but additional rules apply:
     *
     *      only words with single tag prefix are interpreted as tags, so
     *      ##Tag will be interpreted as a part of the name, not as a tag;
     *
     *      single tag prefix (with no characters after it) is also interpreted
     *      as a part of the name.
     *
     * Description can have multiple words with estimated cost prefixes,
     * but only last of them will be interpreted, previous will be interpreted
     * as a part of the name. */
    void addTask(const std::string& encodedDescription) const override;

private:
    CommandHandler<use_cases::CreateTaskCommand>& createTaskHandler;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: ADDTASKCONTROLPRESENTER_H_KQFERPSW */

