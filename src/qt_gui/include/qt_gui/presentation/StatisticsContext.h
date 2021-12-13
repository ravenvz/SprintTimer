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
#ifndef STATISTICSCONTEXT_H_4GUKJLMO
#define STATISTICSCONTEXT_H_4GUKJLMO

#include "core/AsyncQueryHandler.h"
#include "core/Observable.h"
#include "core/TagTop.h"
#include "core/use_cases/SprintMapper.h"
#include "core/use_cases/request_sprints/RequestSprintsQuery.h"
#include <optional>
#include <ranges>

#include "core/use_cases/SprintMapper.h"
#include "core/use_cases/request_statistics/SprintStatisticsDTO.h"

namespace sprint_timer::ui {

class StatisticsContext {
public:
    StatisticsContext() = default;

    explicit StatisticsContext(dw::DateRange dateRange);

    StatisticsContext(use_cases::SprintStatisticsDTO&& data,
                      dw::DateRange dateRange);

    const std::vector<dw::DateTimeRange>& sprintIntervals() const;

    [[nodiscard]] std::vector<std::pair<std::string, double>>
    tagFrequencies() const;

    std::optional<dw::DateRange> currentRange() const;

    void selectTag(std::optional<size_t> tagNumber);

    std::optional<size_t> selectedTag() const;

    size_t numTopTags() const;

private:
    use_cases::SprintStatisticsDTO data;
    std::optional<dw::DateRange> dateRange;
    std::optional<size_t> tag;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: STATISTICSCONTEXT_H_4GUKJLMO */
