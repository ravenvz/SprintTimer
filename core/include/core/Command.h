/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#ifndef ICOMMAND_H_NHSUJX60
#define ICOMMAND_H_NHSUJX60

#include <string>

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;

    virtual void undo() {}

    virtual bool supportUndo() const { return false; }

    virtual std::string inspect() const = 0;
};


#endif /* end of include guard: ICOMMAND_H_NHSUJX60 */
