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
#ifndef REQUESTSPRINTS_H_4QUSWCF0
#define REQUESTSPRINTS_H_4QUSWCF0


#include "core/ISprintStorageReader.h"
#include "core/Query.h"

namespace sprint_timer::use_cases {

class RequestSprints : public Query {
public:
    using QueryResultHandler
        = std::function<void(const std::vector<entities::Sprint>&)>;
    RequestSprints(ISprintStorageReader& sprint_storage_reader,
                   dw::DateRange dateRange,
                   QueryResultHandler resultHandler);

    void execute() final;

    std::string describe() const final;

private:
    ISprintStorageReader& reader;
    const dw::DateRange dateRange_;
    QueryResultHandler handler;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: REQUESTSPRINTS_H_4QUSWCF0 */
