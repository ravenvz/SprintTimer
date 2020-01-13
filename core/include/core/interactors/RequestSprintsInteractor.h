/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#ifndef REQUESTSPRINTSINTERACTOR_H_KUKGEOHT
#define REQUESTSPRINTSINTERACTOR_H_KUKGEOHT

#include "core/ISprintStorageReader.h"
#include "core/QueryInvoker.h"
#include "core/use_cases/RequestSprints.h"

namespace sprint_timer::interactors {

class RequestSprintsInteractor {
public:
    using Handler = use_cases::RequestSprints::QueryResultHandler;

    RequestSprintsInteractor(QueryInvoker& queryInvoker,
                             ISprintStorageReader& reader);

    void request_sprints(dw::DateRange dateRange, Handler handler);

private:
    QueryInvoker& queryInvoker;
    ISprintStorageReader& reader;
};

} // namespace sprint_timer::interactors

#endif /* end of include guard: REQUESTSPRINTSINTERACTOR_H_KUKGEOHT */
