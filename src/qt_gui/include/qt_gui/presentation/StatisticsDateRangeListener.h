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
#ifndef STATISTICSDATERANGELISTENER_H_OMJ6BZ0P
#define STATISTICSDATERANGELISTENER_H_OMJ6BZ0P

#include "qt_gui/presentation/DateRangeChangeListener.h"
#include "qt_gui/presentation/StatisticsContext.h"
#include "qt_gui/presentation/StatisticsMediator.h"

namespace sprint_timer::ui {

class StatisticsDateRangeListener : public DateRangeChangeListener,
                                    public StatisticsColleague {
public:
    StatisticsDateRangeListener(StatisticsMediator& mediator_,
                                StatisticsContext& statisticsContext_)
        : mediator{mediator_}
        , statisticsContext{statisticsContext_}
    {
        mediator.addColleague(this);
    }

    ~StatisticsDateRangeListener() override { mediator.removeColleague(this); }

    void onRangeChanged(const dw::DateRange& range) override
    {
        statisticsContext.selectedRange = range;
        mediator.notifyAll(
            [](StatisticsColleague* col) { col->onSharedDataChanged(); });
    }

private:
    StatisticsMediator& mediator;
    StatisticsContext& statisticsContext;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: STATISTICSDATERANGELISTENER_H_OMJ6BZ0P */
