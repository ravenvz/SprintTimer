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
    request_sprints_hdl_t& requestSprintsHandler_,
    StatisticsMediator& mediator_,
    StatisticsContext& statisticsContext_,
    size_t numTopTags_)
    : requestSprintsHandler{requestSprintsHandler_}
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
    if (!statisticsContext.currentRange()) {
        return;
    }
    data = requestSprintsHandler.handle(
        use_cases::RequestSprintsQuery{*statisticsContext.currentRange()});
}

void StatisticsSharedDataFetcher::updateView()
{
    if (!data || !statisticsContext.currentRange()) {
        return;
    }
    statisticsContext =
        StatisticsContext{*data, *statisticsContext.currentRange(), numTopTags};
    mediator.mediate(this,
                     [](auto* colleague) { colleague->onSharedDataChanged(); });
}

void StatisticsSharedDataFetcher::onDateRangeChanged(const dw::DateRange& range)
{
    const auto sprints =
        requestSprintsHandler.handle(use_cases::RequestSprintsQuery{range});
    statisticsContext = StatisticsContext{sprints, range, numTopTags};
    mediator.mediate(this,
                     [](auto* colleague) { colleague->onSharedDataChanged(); });
}

void StatisticsSharedDataFetcher::onTagSelected(std::optional<size_t> tag)
{
    statisticsContext.selectTag(tag);
}

} // namespace sprint_timer::ui
