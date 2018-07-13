/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#include "core/use_cases/RenameTag.h"

namespace sprint_timer::use_cases {

RenameTag::RenameTag(ITaskStorageWriter& tagStorageWriter,
                     std::string oldName,
                     std::string newName)
    : writer{tagStorageWriter}
    , oldName_{std::move(oldName)}
    , newName_{std::move(newName)}
{
}

void RenameTag::execute() { writer.editTag(oldName_, newName_); }

void RenameTag::undo() { writer.editTag(newName_, oldName_); }

std::string RenameTag::describe() const
{
    std::stringstream ss;
    ss << "Edit tag (" << oldName_ << " -> " << newName_ << ")";
    return ss.str();
}

} // namespace sprint_timer::use_cases
