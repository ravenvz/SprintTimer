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

#include "core/use_cases/RequestAllTags.h"

namespace UseCases {

RequestAllTags::RequestAllTags(
    ITaskStorageReader& taskStorageReader,
    std::function<void(const std::vector<std::string>&)> handler)
    : reader{taskStorageReader}
    , handler{handler}
{
}

void RequestAllTags::execute() { reader.requestAllTags(handler); }

std::string RequestAllTags::inspect() const { return "Request all tags"; }

} /* UseCases */
