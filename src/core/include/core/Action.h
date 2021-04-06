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
#ifndef ACTION_H_LZE48PW5
#define ACTION_H_LZE48PW5

#include <string>

namespace sprint_timer {

/* Note that Action is a Command in classical Command pattern.
 * This name was choosen so that it would not be confused with user
 * commands. Actions are used in core module internally and are not
 * ment to be used by core module clients directly. */
class Action {
public:
    virtual ~Action() = default;

    virtual void execute() = 0;

    virtual void undo() = 0;

    virtual std::string describe() const = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: ACTION_H_LZE48PW5 */

