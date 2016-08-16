/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef ADDPOMODOROTRANSACTION_H_YUMZVLHC
#define ADDPOMODOROTRANSACTION_H_YUMZVLHC

#include "core/IPomodoroStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class RegisterNewPomodoro : public RevertableCommand {
public:
    RegisterNewPomodoro(IPomodoroStorageWriter& writer,
                        const Sprint& pomodoro)
        : writer{writer}
        , pomodoro{pomodoro}
    {
    }

    void executeAction() final { writer.save(pomodoro); }

    void undoAction() final { writer.remove(pomodoro); }

    std::string inspect() const final
    {
        return "Register new pomodoro '" + pomodoro.toString() + "'";
    }

private:
    IPomodoroStorageWriter& writer;
    const Sprint pomodoro;
};

} /* UseCases */


#endif /* end of include guard: ADDPOMODOROTRANSACTION_H_YUMZVLHC */
