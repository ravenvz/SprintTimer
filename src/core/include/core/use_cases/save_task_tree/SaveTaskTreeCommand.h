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
#ifndef SAVETASKTREE_H_N4GBFEPQ
#define SAVETASKTREE_H_N4GBFEPQ

#include "core/Command.h"
#include "core/use_cases/TaskTreeDTO.h"

namespace sprint_timer::use_cases {

struct SaveTaskTreeCommand : public Command {

    explicit SaveTaskTreeCommand(TaskTreeDTO&& taskTree)
        : taskTree{std::move(taskTree)}
    {
    }

    TaskTreeDTO taskTree;

    friend bool operator==(const SaveTaskTreeCommand&,
                           const SaveTaskTreeCommand&) = default;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: SAVETASKTREE_H_N4GBFEPQ */
