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
#ifndef STATISTICSSHAREDDATAFETCHER_H_SCYAK0IT
#define STATISTICSSHAREDDATAFETCHER_H_SCYAK0IT

#include "core/AsyncQueryHandler.h"
#include "core/use_cases/request_sprints/RequestSprintsQuery.h"
#include "qt_gui/mvp/DataFetcher.h"
#include "qt_gui/presentation/StatisticsColleague.h"
#include "qt_gui/presentation/StatisticsContext.h"
#include "qt_gui/presentation/StatisticsMediator.h"

namespace sprint_timer::ui {

class StatisticsSharedDataFetcher : public mvp::DataFetcher,
                                    public StatisticsColleague {
public:
    using request_sprints_hdl_t =
        AsyncQueryHandler<use_cases::RequestSprintsQuery>;

    StatisticsSharedDataFetcher(request_sprints_hdl_t& requestSprintsHandler_,
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

    ~StatisticsSharedDataFetcher() { mediator.removeColleague(this); }

    void fetchData() override
    {
        if (!statisticsContext.currentRange()) {
            return;
        }
        dataFuture = requestSprintsHandler.handle(
            use_cases::RequestSprintsQuery{*statisticsContext.currentRange()});
    }

    void updateView() override
    {
        if (!dataFuture.valid() || !statisticsContext.currentRange()) {
            return;
        }
        statisticsContext = StatisticsContext{
            dataFuture.get(), *statisticsContext.currentRange(), numTopTags};
        mediator.mediate(
            this, [](auto* colleague) { colleague->onSharedDataChanged(); });
    }

    void onDateRangeChanged(const dw::DateRange& range) override
    {
        const auto sprints = requestSprintsHandler.handleSync(
            use_cases::RequestSprintsQuery{range});
        statisticsContext = StatisticsContext{sprints, range, numTopTags};
        mediator.mediate(
            this, [](auto* colleague) { colleague->onSharedDataChanged(); });
    }

    void onTagSelected(std::optional<size_t> tag) override
    {
        statisticsContext.selectTag(tag);
    }

private:
    request_sprints_hdl_t& requestSprintsHandler;
    StatisticsMediator& mediator;
    StatisticsContext& statisticsContext;
    size_t numTopTags;
    std::future<use_cases::RequestSprintsQuery::result_t> dataFuture;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: STATISTICSSHAREDDATAFETCHER_H_SCYAK0IT */

