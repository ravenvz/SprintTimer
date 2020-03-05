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
#ifndef BESTWORKDAYCONTRACT_H_PNBSPBF6
#define BESTWORKDAYCONTRACT_H_PNBSPBF6

#include <string>
#include <vector>

namespace sprint_timer::ui::contracts::BestWorkday {

class View {
public:
    struct BarD {
        std::string borderColor;
        std::string barColor;
        std::vector<double> barValues;
        std::vector<int> dayOrder;
    };

    struct LegendData {
        int dayNum{-1};
        std::string percentage;
    };

    virtual ~View() = default;

    virtual void displayBars(const BarD& data) = 0;

    virtual void displayLegend(const LegendData& data) = 0;
};

} // namespace sprint_timer::ui::contracts::BestWorkday

#endif /* end of include guard: BESTWORKDAYCONTRACT_H_PNBSPBF6 */
