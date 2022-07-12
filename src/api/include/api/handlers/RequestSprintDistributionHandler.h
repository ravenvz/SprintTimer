/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#ifndef REQUESTSPRINTDISTRIBUTIONHANDLER_H_UFD8ONGB
#define REQUESTSPRINTDISTRIBUTIONHANDLER_H_UFD8ONGB

#include "api/com_query/QueryHandler.h"
#include "api/requests/RequestSprintDistributionQuery.h"
#include "core/SprintDistributionReader.h"

namespace sprint_timer::api {

class RequestSprintDistributionHandler
    : public asp::QueryHandler<RequestSprintDistributionQuery> {
public:
    explicit RequestSprintDistributionHandler(SprintDistributionReader& reader);

    std::vector<int>
    handle(const RequestSprintDistributionQuery& query) override;

private:
    SprintDistributionReader& reader;
};

} // namespace sprint_timer::api

#endif /* end of include guard: REQUESTSPRINTDISTRIBUTIONHANDLER_H_UFD8ONGB */
