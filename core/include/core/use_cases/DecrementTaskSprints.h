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
#ifndef DECREMENTTASKSPRINTS_H_GYPTACBX
#define DECREMENTTASKSPRINTS_H_GYPTACBX

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class DecrementTaskSprints : public RevertableCommand {
public:
    DecrementTaskSprints(ITaskStorageWriter& taskStorageWriter,
                         const std::string& taskUuid);

    std::string inspect() const final;

protected:
    void executeAction() final;

    void undoAction() final;

private:
    ITaskStorageWriter& writer;
    const std::string taskUuid;
};

} /* UseCases */

#endif /* end of include guard: DECREMENTSPENTSPRINTS_H_GYPTACBX */
