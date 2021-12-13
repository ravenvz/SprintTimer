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
#ifndef BESTWORKDAYPRESENTER_H_OIE4UYUT
#define BESTWORKDAYPRESENTER_H_OIE4UYUT

#include "core/Distribution.h"
#include "core/QueryHandler.h"
#include "core/use_cases/request_statistics/WorkdayStatisticsQuery.h"
#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/presentation/BestWorkdayContract.h"
#include "qt_gui/presentation/StatisticsContext.h"
#include "qt_gui/presentation/StatisticsMediator.h"

namespace sprint_timer::ui {

class BestWorkdayPresenter : public contracts::BestWorkday::Presenter,
                             public StatisticsColleague {
public:
    using workday_statistics_handler_t =
        QueryHandler<use_cases::WorkdayStatisticsQuery>;

    BestWorkdayPresenter(workday_statistics_handler_t& workdayStatisticsHandler,
                         StatisticsMediator& mediator,
                         const StatisticsContext& statisticsContext,
                         dw::Weekday firstDayOfWeek);

    ~BestWorkdayPresenter() override;

    void onSharedDataChanged() override;

    BestWorkdayPresenter(BestWorkdayPresenter&& other) = default;

    BestWorkdayPresenter& operator=(BestWorkdayPresenter&& other) = default;

private:
    std::reference_wrapper<workday_statistics_handler_t>
        workdayStatisticsHandler;
    std::reference_wrapper<StatisticsMediator> mediator;
    std::reference_wrapper<const StatisticsContext> statisticsContext;
    dw::Weekday firstDayOfWeek;
    std::optional<use_cases::WorkdayStatisticsDTO> workdayStatistics;

    void fetchDataImpl() override;

    void updateViewImpl() override;

    void updateLegend(dw::Weekday bestWorkday,
                      int percentageAboveAverage) const;

    void updateBars(std::array<double, 7> weekdayDistribution) const;

    void updateWithDefaultValues() const;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: BESTWORKDAYPRESENTER_H_OIE4UYUT */
