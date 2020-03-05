/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#ifndef DAILYSTATISTICSGRAPHPRESENTER_H_8VCTBWMG
#define DAILYSTATISTICSGRAPHPRESENTER_H_8VCTBWMG

#include "qt_gui/presentation/BasePresenter.h"
#include "qt_gui/presentation/DailyStatisticsGraphContract.h"
#include "qt_gui/presentation/StatisticsMediator.h"
#include <core/use_cases/request_schedule/WorkScheduleHandler.h>

namespace sprint_timer::ui {

class DailyStatisticsGraphPresenter
    : public BasePresenter<contracts::DailyStatisticGraphContract::View>,
      public StatisticsColleague {
public:
    DailyStatisticsGraphPresenter(
        QueryHandler<use_cases::WorkScheduleQuery, WorkSchedule>&
            workScheduleHandler,
        StatisticsMediator& mediator);

    ~DailyStatisticsGraphPresenter() override;

    void onSharedDataChanged() override;

private:
    QueryHandler<use_cases::WorkScheduleQuery, WorkSchedule>&
        workScheduleHandler;
    StatisticsMediator& mediator;

    void updateViewImpl() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: DAILYSTATISTICSGRAPHPRESENTER_H_8VCTBWMG */
