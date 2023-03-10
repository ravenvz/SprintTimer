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
#ifndef CHANGETASKSPRIORITIES_H_MDWA0JUJ
#define CHANGETASKSPRIORITIES_H_MDWA0JUJ

#include "api/TaskStorageWriter.h"

namespace sprint_timer::api::actions {

class ChangeTasksPriorities {
public:
    ChangeTasksPriorities(TaskStorageWriter& taskStorageWriter_,
                          std::vector<std::string> oldOrder_,
                          std::vector<std::string> newOrder_);

    auto execute() -> void;

    auto undo() -> void;

    [[nodiscard]] auto describe() const -> std::string;

private:
    TaskStorageWriter& writer;
    std::vector<std::string> oldOrder;
    std::vector<std::string> newOrder;
};

} // namespace sprint_timer::api::actions

#endif /* end of include guard: CHANGETASKSPRIORITIES_H_MDWA0JUJ */

