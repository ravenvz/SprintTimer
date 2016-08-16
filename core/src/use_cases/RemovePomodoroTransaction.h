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
#ifndef REMOVEPOMODOROTRANSACTION_H_GAR8SZLM
#define REMOVEPOMODOROTRANSACTION_H_GAR8SZLM

#include "core/IPomodoroStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class RemovePomodoroTransaction : public RevertableCommand {
public:
    RemovePomodoroTransaction(IPomodoroStorageWriter& writer,
                              const Sprint& pomodoro)
        : writer{writer}
        , pomodoro{pomodoro}
    {
    }

    void executeAction() final { writer.remove(pomodoro); }

    void undoAction() final { writer.save(pomodoro); }

    std::string inspect() const final
    {
        return "Remove Sprint '" + pomodoro.toString() + "'";
    }

private:
    IPomodoroStorageWriter& writer;
    const Sprint pomodoro;
};

} // namespace UseCases

#endif /* end of include guard: REMOVEPOMODOROTRANSACTION_H_GAR8SZLM */
