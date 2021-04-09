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
#ifndef RENAMETAG_H_5X4BJA83
#define RENAMETAG_H_5X4BJA83

#include "core/Action.h"
#include "core/TaskStorageWriter.h"
#include <iostream>

namespace sprint_timer::actions {

class RenameTag : public Action {
public:
    RenameTag(TaskStorageWriter& tagStorageWriter,
              std::string oldName,
              std::string newName);

    void execute() final;

    void undo() final;

    std::string describe() const final;

private:
    TaskStorageWriter& writer;
    const std::string oldName_;
    const std::string newName_;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: RENAMETAG_H_5X4BJA83 */

