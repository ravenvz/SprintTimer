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
#ifndef DAYTIMESTATISTICSPRESENTER_H_XI31IPVZ
#define DAYTIMESTATISTICSPRESENTER_H_XI31IPVZ

#include "qt_gui/presentation/BasePresenter.h"
#include "qt_gui/presentation/DaytimeStatisticsContract.h"
#include "qt_gui/presentation/StatisticsMediator.h"

namespace sprint_timer::ui {

class DaytimeStatisticsPresenter
    : public BasePresenter<contracts::DaytimeStatisticsContract::View>,
      public StatisticsColleague {
public:
    explicit DaytimeStatisticsPresenter(StatisticsMediator& mediator);

    ~DaytimeStatisticsPresenter() override;

    void onSharedDataChanged() override;

private:
    StatisticsMediator& mediator;

    void updateViewImpl() override;
};

struct SprintDaytimeDistribution {
    sprint_timer::Distribution<double> dayPartDistribution;
    std::vector<dw::DateTimeRange> timeRanges;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: DAYTIMESTATISTICSPRESENTER_H_XI31IPVZ */
