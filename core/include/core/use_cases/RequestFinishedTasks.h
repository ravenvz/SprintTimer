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
#ifndef REQUESTFINISHEDTASKSCOMMAND_H_AF6E0NUL
#define REQUESTFINISHEDTASKSCOMMAND_H_AF6E0NUL

#include "core/Command.h"
#include "core/ITaskStorageReader.h"

namespace UseCases {

class RequestFinishedTasks : public Command {
public:
    RequestFinishedTasks(ITaskStorageReader& taskStorageReader,
                         const dw::TimeSpan& timeSpan,
                         ITaskStorageReader::Handler handler);

    void execute() final;

    std::string inspect() const final;

private:
    ITaskStorageReader& reader;
    const dw::TimeSpan timeSpan;
    ITaskStorageReader::Handler handler;
};

} /* UseCases */

#endif /* end of include guard: REQUESTFINISHEDTASKSCOMMAND_H_AF6E0NUL */
