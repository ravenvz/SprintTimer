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
#ifndef DAILYPROGRESSPRESENTERCONTRACT_H_ERA0VCQQ
#define DAILYPROGRESSPRESENTERCONTRACT_H_ERA0VCQQ

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include "core/ProgressOverPeriod.h"

namespace sprint_timer::ui::contracts::DailyProgress {

struct LegendData {
    std::string count;
    std::string left;
    std::string difference;
    std::string average;
    std::string percentage;
};

struct GaugeValues {
    int estimated;
    int actual;
    std::string emptyColor;
    std::string filledColor;
};

struct ProgressBarData {
    int estimated;
    int actual;
    std::string emptyColor;
    std::string filledColor;
    bool isVisible;
};

class View : public mvp::BaseView<View, mvp::BasePresenter<View>> {
public:
    virtual void displayLegend(const LegendData& data) = 0;

    virtual void displayGauges(const std::vector<GaugeValues>& data) = 0;

    virtual void displayProgressBar(const ProgressBarData& data) = 0;
};

} // namespace sprint_timer::ui::contracts::DailyProgress

#endif /* end of include guard: DAILYPROGRESSPRESENTERCONTRACT_H_ERA0VCQQ */

