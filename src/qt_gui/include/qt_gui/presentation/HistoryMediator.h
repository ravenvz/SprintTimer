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
#ifndef HISTORYMEDIATOR_H_RLAZPAX0
#define HISTORYMEDIATOR_H_RLAZPAX0

#include "qt_gui/presentation/DateRangeChangeListener.h"
#include "qt_gui/presentation/Mediator.h"

namespace sprint_timer::ui {

class HistoryColleague {
public:
    virtual ~HistoryColleague() = default;

    virtual void onSharedDataChanged() = 0;
};

class HistoryMediator : public Mediator<HistoryColleague>,
                        public DateRangeChangeListener {
public:
    enum class DisplayedHistory { SprintHistory, TaskHistory };

    virtual void changeDisplayedHistory(HistoryColleague* caller,
                                        DisplayedHistory displayedHistory) = 0;

    virtual std::optional<dw::DateRange> currentDateRange() const = 0;

    virtual DisplayedHistory displayedHistory() const = 0;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: HISTORYMEDIATOR_H_RLAZPAX0 */
