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
#ifndef BESTDAYTIMEPRESENTER_H_DDRXAWZC
#define BESTDAYTIMEPRESENTER_H_DDRXAWZC

#include "core/Distribution.h"
#include "core/QueryHandler.h"
#include "core/use_cases/request_statistics/WorktimeStatisticsQuery.h"
#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/presentation/BestWorktimeContract.h"
#include "qt_gui/presentation/StatisticsContext.h"
#include "qt_gui/presentation/StatisticsMediator.h"

namespace sprint_timer::ui {

class BestWorktimePresenter
    : public mvp::BasePresenter<contracts::BestWorktimeContract::View>,
      public StatisticsColleague {
public:
    using worktime_statistics_handler_t =
        QueryHandler<use_cases::WorktimeStatisticsQuery>;

    BestWorktimePresenter(
        worktime_statistics_handler_t& worktimeStatisticsHandler,
        StatisticsMediator& mediator,
        const StatisticsContext& statisticsContext);

    ~BestWorktimePresenter() override;

    void onSharedDataChanged() override;

private:
    worktime_statistics_handler_t& worktimeStatisticsHandler;
    StatisticsMediator& mediator;
    const StatisticsContext& statisticsContext;
    std::optional<use_cases::WorktimeStatisticsDTO> worktimeStatistics;

    void fetchDataImpl() override;

    void updateViewImpl() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: BESTDAYTIMEPRESENTER_H_DDRXAWZC */

