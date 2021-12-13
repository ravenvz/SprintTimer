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
#include "qt_gui/presentation/StatisticsContext.h"
#include <ranges>

namespace sprint_timer::ui {

StatisticsContext::StatisticsContext(dw::DateRange dateRange_)
    : dateRange{dateRange_}
{
}

StatisticsContext::StatisticsContext(use_cases::SprintStatisticsDTO&& data_,
                                     dw::DateRange dateRange_)
    : data{std::move(data_)}
    , dateRange{dateRange_}
{
}

const std::vector<dw::DateTimeRange>& StatisticsContext::sprintIntervals() const
{
    return tag ? data.data[*tag].sprints : data.allSprints;
}

std::vector<std::pair<std::string, double>>
StatisticsContext::tagFrequencies() const
{
    std::vector<std::pair<std::string, double>> tagFrequencies;
    tagFrequencies.reserve(data.numTopTags);
    std::ranges::copy(std::views::transform(
                          data.data,
                          [](const auto& el) -> std::pair<std::string, double> {
                              return {el.tag, el.frequency};
                          }),
                      std::back_inserter(tagFrequencies));
    return tagFrequencies;
}

std::optional<dw::DateRange> StatisticsContext::currentRange() const
{
    return dateRange;
}

void StatisticsContext::selectTag(std::optional<size_t> tagNumber)
{
    tag = tagNumber;
}

std::optional<size_t> StatisticsContext::selectedTag() const { return tag; }

size_t StatisticsContext::numTopTags() const { return data.numTopTags; }

} // namespace sprint_timer::ui
