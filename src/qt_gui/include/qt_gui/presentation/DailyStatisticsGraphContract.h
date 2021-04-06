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
#ifndef DAILYSTATISTICSGRAPHCONTRACT_H_KSDC1OBX
#define DAILYSTATISTICSGRAPHCONTRACT_H_KSDC1OBX

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include <string>
#include <vector>

namespace sprint_timer::ui::contracts::DailyStatisticGraphContract {

struct LegendData {
    std::string total;
    std::string average;
};

struct DayNumber {
    unsigned value;
};

struct Value {
    double value;
};

struct Values {
    DayNumber dayNumber;
    Value value;
};

struct GraphValue {
    Value xValue;
    Value yValue;
    std::string label;
};

enum class LineStyle { Solid, Dot, Dash };

struct GraphOptions {
    std::string penColor;
    double penWidth;
    bool showPoints;
    std::string pointColor;
    LineStyle style;
};

// struct GraphData {
//     GraphOptions options;
//     std::vector<std::pair<DayNumber, Value>> data;
// };

struct GraphData {
    GraphOptions options;
    std::vector<GraphValue> values;
};

class View : public mvp::BaseView<View, mvp::BasePresenter<View>> {
public:
    virtual void updateLegend(const LegendData& data) = 0;

    virtual void clearGraphs() = 0;

    virtual void drawGraph(const GraphData& data) = 0;
};

} // namespace sprint_timer::ui::contracts::DailyStatisticGraphContract

#endif /* end of include guard: DAILYSTATISTICSGRAPHCONTRACT_H_KSDC1OBX */
