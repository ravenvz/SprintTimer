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
#include "qt_gui/presentation/RegisterSprintControlPresenter.h"

namespace sprint_timer::ui {

RegisterSprintControlPresenter::RegisterSprintControlPresenter(
    register_sprint_bulk_hdl_t& registerSprintBulkHandler_)
    : registerSprintBulkHandler{registerSprintBulkHandler_}
{
}

void RegisterSprintControlPresenter::registerConsecutiveSprints(
    const std::string& taskUuid,
    dw::DateTime firstSprintStart,
    int32_t numSprints,
    std::chrono::minutes sprintDuration)
{
    dw::DateTimeRange span{firstSprintStart, firstSprintStart + sprintDuration};
    std::vector<dw::DateTimeRange> intervals(static_cast<size_t>(numSprints),
                                             span);
    std::generate(
        begin(intervals), end(intervals), [&, iteration = 0]() mutable {
            return dw::add_offset(span, iteration++ * sprintDuration);
        });
    registerSprintBulkHandler.handle(
        use_cases::RegisterSprintBulkCommand{taskUuid, intervals});
}

void RegisterSprintControlPresenter::registerSprintBulk(
    const std::string& taskUuid,
    const std::vector<dw::DateTimeRange>& timeRanges)
{
    registerSprintBulkHandler.handle(
        use_cases::RegisterSprintBulkCommand{taskUuid, timeRanges});
}

} // namespace sprint_timer::ui
