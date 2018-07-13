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
#ifndef REQUESTALLTAGS_H_CUXYESDS
#define REQUESTALLTAGS_H_CUXYESDS

#include "core/ITaskStorageReader.h"
#include "core/Query.h"

namespace sprint_timer::use_cases {

class RequestAllTags : public Query {
public:
    using QueryResultHandler
        = std::function<void(const std::vector<std::string>&)>;

    RequestAllTags(ITaskStorageReader& taskStorageReader,
                   QueryResultHandler handler);

    void execute() final;

    std::string describe() const final;

private:
    ITaskStorageReader& reader;
    QueryResultHandler handler_;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: REQUESTALLTAGS_H_CUXYESDS */
