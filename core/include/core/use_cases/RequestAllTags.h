/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#ifndef REQUESTALLTAGS_H_CUXYESDS
#define REQUESTALLTAGS_H_CUXYESDS

#include "core/Command.h"
#include "core/ITaskStorageReader.h"

namespace UseCases {

class RequestAllTags : public Command {
public:
    RequestAllTags(
        ITaskStorageReader& taskStorageReader,
        std::function<void(const std::vector<std::string>&)> handler);

    void execute() final;

    std::string inspect() const final;

private:
    ITaskStorageReader& reader;
    std::function<void(const std::vector<std::string>&)> handler;
};

} /* UseCases */

#endif /* end of include guard: REQUESTALLTAGS_H_CUXYESDS */
