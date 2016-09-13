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
#ifndef REQUESTSPRINTS_H_4QUSWCF0
#define REQUESTSPRINTS_H_4QUSWCF0


#include "core/Command.h"
#include "core/ISprintStorageReader.h"

namespace UseCases {

class RequestSprints : public Command {
public:
    RequestSprints(
        ISprintStorageReader& reader,
        const TimeSpan& timeSpan,
        std::function<void(const std::vector<Sprint>&)> resultHandler);

    void execute() final;

    std::string inspect() const final;

private:
    ISprintStorageReader& reader;
    const TimeSpan timeSpan;
    std::function<void(const std::vector<Sprint>&)> handler;
};

} /* UseCases */

#endif /* end of include guard: REQUESTSPRINTS_H_4QUSWCF0 */
