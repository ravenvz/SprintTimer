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
#ifndef EDITTAG_H_6NGOS1Q2
#define EDITTAG_H_6NGOS1Q2

#include "core/Command.h"
#include "core/ITaskStorageWriter.h"
#include <iostream>

namespace core::use_cases {

class RenameTag : public Command {
public:
    RenameTag(ITaskStorageWriter& tagStorageWriter,
              std::string oldName,
              std::string newName);

    void execute() final;

    void undo() final;

    std::string describe() const final;

private:
    ITaskStorageWriter& writer;
    const std::string oldName_;
    const std::string newName_;
};

} // namespace core::use_cases

#endif /* end of include guard: EDITTAG_H_6NGOS1Q2 */
