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
#include "qt_gui/presentation/StatisticsSharedDataFetcher.h"

namespace sprint_timer::ui {

StatisticsSharedDataFetcher::StatisticsSharedDataFetcher(
    sprint_statistics_hdl_t& sprintStatisticsHandler_,
    StatisticsMediator& mediator_,
    StatisticsContext& statisticsContext_,
    size_t numTopTags_)
    : sprintStatisticsHandler{sprintStatisticsHandler_}
    , mediator{mediator_}
    , statisticsContext{statisticsContext_}
    , numTopTags{numTopTags_}
{
    mediator.addColleague(this);
}

StatisticsSharedDataFetcher::~StatisticsSharedDataFetcher()
{
    mediator.removeColleague(this);
}

void StatisticsSharedDataFetcher::fetchData()
{
    if (auto dateRange = statisticsContext.currentRange(); dateRange) {
        statisticsContext = StatisticsContext{
            sprintStatisticsHandler.handle(
                use_cases::SprintStatisticsQuery{numTopTags, *dateRange}),
            *statisticsContext.currentRange()};
    }
}

void StatisticsSharedDataFetcher::updateView()
{
    if (!statisticsContext.currentRange()) {
        return;
    }
    mediator.mediate(this,
                     [](auto* colleague) { colleague->onSharedDataChanged(); });
}

void StatisticsSharedDataFetcher::onDateRangeChanged(const dw::DateRange& range)
{
    statisticsContext = StatisticsContext{range};
    fetchData();
    updateView();
}

void StatisticsSharedDataFetcher::onTagSelected(std::optional<size_t> tag)
{
    statisticsContext.selectTag(tag);
}

} // namespace sprint_timer::ui
