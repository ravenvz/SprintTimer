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

#include "api/actions/RenameTag.h"

namespace sprint_timer::api::actions {

RenameTag::RenameTag(TaskStorageWriter& tagStorageWriter,
                     std::string oldName,
                     std::string newName)
    : writer{tagStorageWriter}
    , oldName_{std::move(oldName)}
    , newName_{std::move(newName)}
{
}

auto RenameTag::execute() -> void { writer.editTag(oldName_, newName_); }

auto RenameTag::undo() -> void { writer.editTag(newName_, oldName_); }

auto RenameTag::describe() const -> std::string
{
    std::stringstream ss;
    ss << "Edit tag (" << oldName_ << " -> " << newName_ << ")";
    return ss.str();
}

} // namespace sprint_timer::api::actions
