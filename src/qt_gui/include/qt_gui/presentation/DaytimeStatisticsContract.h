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
#ifndef DAYTIMESTATISTICSCONTRACT_H_R72BBVOY
#define DAYTIMESTATISTICSCONTRACT_H_R72BBVOY

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include <date_wrapper/date_wrapper.h>
#include <string>
#include <vector>

namespace sprint_timer::ui::contracts::DaytimeStatisticsContract {

struct LegendData {
    std::string periodName;
    std::string periodHours;
};

struct DiagramData {
    std::string filledColor;
    std::vector<dw::DateTimeRange> timeRanges;
};

class View : public mvp::BaseView<View, mvp::BasePresenter<View>> {
public:
    virtual void updateLegend(const LegendData& data) = 0;

    virtual void updateDiagram(const DiagramData& data) = 0;
};

} // namespace sprint_timer::ui::contracts::DaytimeStatisticsContract

#endif /* end of include guard: DAYTIMESTATISTICSCONTRACT_H_R72BBVOY */
