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
#ifndef BESTWORKDAYCONTRACT_H_HPKS73X0
#define BESTWORKDAYCONTRACT_H_HPKS73X0

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include <array>
#include <span>
#include <string>
#include <vector>

namespace sprint_timer::ui::contracts::BestWorkday {

class Presenter;

class View : public mvp::BaseView<View, Presenter> {
public:
    struct BarD {
        std::string borderColor;
        std::string barColor;
        std::span<const double, 7> barValues;
        std::span<const int, 7> dayOrder;
    };

    struct LegendData {
        int dayNum{-1};
        std::string percentage;
    };

    virtual void displayBars(const BarD& data) = 0;

    virtual void displayLegend(const LegendData& data) = 0;
};

class Presenter : public mvp::BasePresenter<View> {
};

} // namespace sprint_timer::ui::contracts::BestWorkday

#endif /* end of include guard: BESTWORKDAYCONTRACT_H_HPKS73X0 */

