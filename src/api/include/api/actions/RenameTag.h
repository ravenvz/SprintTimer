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

#include "api/TaskStorageWriter.h"
#include <iostream>

namespace sprint_timer::api::actions {

class RenameTag {
public:
    RenameTag(TaskStorageWriter& tagStorageWriter,
              std::string oldName,
              std::string newName);

    auto execute() -> void;

    auto undo() -> void;

    [[nodiscard]] auto describe() const -> std::string;

private:
    TaskStorageWriter& writer;
    const std::string oldName_;
    const std::string newName_;
};

} // namespace sprint_timer::api::actions

#endif /* end of include guard: RENAMETAG_H_5X4BJA83 */

