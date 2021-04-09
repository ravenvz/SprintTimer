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
#include "qt_gui/presentation/StatisticsMediatorImpl.h"

namespace sprint_timer::ui {

StatisticsMediatorImpl::StatisticsMediatorImpl(
    request_sprints_hdl_t& queryHandler_, size_t numTopTags_)
    : queryHandler{queryHandler_}
    , numTopTags{numTopTags_}
{
    if (dateRange)
        queryHandler.handle(use_cases::RequestSprintsQuery{*dateRange});
}

// TODO workaround until statistics view is streamlined
void StatisticsMediatorImpl::updateView()
{
    if (!dateRange) {
        return;
    }
    currentTagNumber = std::nullopt;
    allSprints =
        queryHandler.handle(use_cases::RequestSprintsQuery{*dateRange});
    tagtop = TagTop{allSprints, numTopTags};
    notifyAll([](auto* colleague) { colleague->onSharedDataChanged(); });
}

void StatisticsMediatorImpl::filterByTag(StatisticsColleague* caller,
                                         std::optional<size_t> tagNumber)
{
    currentTagNumber = tagNumber;
    mediate(caller, [](auto* colleague) { colleague->onSharedDataChanged(); });
}

void StatisticsMediatorImpl::onRangeChanged(const dw::DateRange& range)
{
    dateRange = range;
    currentTagNumber = std::nullopt;
    allSprints = queryHandler.handle(use_cases::RequestSprintsQuery(range));
    tagtop = TagTop{allSprints, numTopTags};
    notifyAll([](auto* colleague) { colleague->onSharedDataChanged(); });
}

const std::vector<entities::Sprint>& StatisticsMediatorImpl::sprints() const
{
    if (currentTagNumber)
        return tagtop.sprintsForTagAt(*currentTagNumber);
    return allSprints;
}

const std::vector<TagTop::TagFrequency>&
StatisticsMediatorImpl::tagFrequencies() const
{
    return tagtop.tagFrequencies();
}

void StatisticsMediatorImpl::changeNumTopTags(size_t numTopTags_)
{
    numTopTags = numTopTags_;
}

std::optional<dw::DateRange> StatisticsMediatorImpl::range() const
{
    return dateRange;
}

std::optional<size_t> StatisticsMediatorImpl::selectedTagNumber() const
{
    return currentTagNumber;
}

} // namespace sprint_timer::ui
