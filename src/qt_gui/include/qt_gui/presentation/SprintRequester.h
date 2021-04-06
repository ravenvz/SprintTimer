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
#ifndef SPRINTREQUESTER_H_JO239RMT
#define SPRINTREQUESTER_H_JO239RMT

#include "qt_gui/presentation/SharedValue.h"
#include <core/QueryHandler.h>
#include <core/use_cases/request_sprints/RequestSprintsQuery.h>

namespace sprint_timer::ui {

class SprintRequester : public Observer {
public:
    SprintRequester(QueryHandler<use_cases::RequestSprintsQuery,
                                 std::vector<entities::Sprint>>& handler,
                    SharedValue<dw::DateRange>& sharedDateRange)
        : handler{handler}
        , sharedDateRange{sharedDateRange}
    {
        sharedDateRange.attach(*this);
    }

    ~SprintRequester() override { sharedDateRange.detach(*this); }

    void update() override
    {
        if (!sharedDateRange.value())
            return;
        data = handler.handle(
            use_cases::RequestSprintsQuery(*sharedDateRange.value()));
    }

    const std::vector<entities::Sprint>& sprints() const { return data; }

    std::optional<dw::DateRange> currentRange() const
    {
        return sharedDateRange.value();
    }

private:
    QueryHandler<use_cases::RequestSprintsQuery, std::vector<entities::Sprint>>&
        handler;
    SharedValue<dw::DateRange>& sharedDateRange;
    std::vector<entities::Sprint> data;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: SPRINTREQUESTER_H_JO239RMT */
