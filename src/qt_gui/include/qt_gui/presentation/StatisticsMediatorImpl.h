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
#ifndef STATISTICSMEDIATORIMPL_H_02PSRLBP
#define STATISTICSMEDIATORIMPL_H_02PSRLBP

#include "core/AsyncQueryHandler.h"
#include "core/QueryHandler.h"
#include "qt_gui/presentation/StatisticsMediator.h"

// TODO workaround until statistics view is streamlined
#include "qt_gui/mvp/AbstractPresenter.h"

namespace sprint_timer::ui {

// TODO workaround until statistics view is streamlined
class StatisticsMediatorImpl : public StatisticsMediator,
                               public mvp::AbstractPresenter {
public:
    using request_sprints_hdl_t = QueryHandler<use_cases::RequestSprintsQuery>;
    using async_request_sprints_hdl_t =
        AsyncQueryHandler<use_cases::RequestSprintsQuery>;

    StatisticsMediatorImpl(request_sprints_hdl_t& queryHandler_,
                           async_request_sprints_hdl_t& asyncQueryHandler_,
                           size_t numTopTags_);

    // TODO workaround until statistics view is streamlined
    void updateView() override;

    void filterByTag(StatisticsColleague* caller,
                     std::optional<size_t> tagNumber) override;

    void onRangeChanged(const dw::DateRange& range) override;

    const std::vector<entities::Sprint>& sprints() const override;

    const std::vector<TagTop::TagFrequency>& tagFrequencies() const override;

    void changeNumTopTags(size_t numTopTags_) override;

    std::optional<dw::DateRange> range() const override;

    std::optional<size_t> selectedTagNumber() const override;

private:
    // request_sprints_hdl_t& queryHandler;
    async_request_sprints_hdl_t& asyncQueryHandler;
    size_t numTopTags;
    std::vector<entities::Sprint> allSprints;
    TagTop tagtop;
    std::optional<dw::DateRange> dateRange;
    std::optional<size_t> currentTagNumber;

    void requestData();

    void onDataReadyCallback(std::vector<entities::Sprint> sprints);
};

} // namespace sprint_timer::ui

#endif /* end of include guard: STATISTICSMEDIATORIMPL_H_02PSRLBP */
