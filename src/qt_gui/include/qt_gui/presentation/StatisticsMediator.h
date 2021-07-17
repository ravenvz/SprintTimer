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
#ifndef STATISTICSMEDIATOR_H_3CXN8PXD
#define STATISTICSMEDIATOR_H_3CXN8PXD

#include "core/TagTop.h"
#include "date_wrapper/date_wrapper.h"
#include "qt_gui/presentation/DateRangeChangeListener.h"
#include "qt_gui/presentation/Mediator.h"
#include "qt_gui/presentation/StatisticsColleague.h"
#include <optional>
#include <string>

namespace sprint_timer::ui {

class StatisticsMediator : public Mediator<StatisticsColleague>,
                           public DateRangeChangeListener {
public:
    virtual void changeNumTopTags(size_t /*numTopTags_*/) { }

    void selectTag(StatisticsColleague* caller, std::optional<size_t> tagNumber)
    {
        mediate(caller, [tag = tagNumber](auto* colleague) {
            colleague->onTagSelected(tag);
        });
        mediate(caller,
                [](auto* colleague) { colleague->onSharedDataChanged(); });
    }

    void onRangeChanged(const dw::DateRange& range) override
    {
        notifyAll(
            [&](auto* colleague) { colleague->onDateRangeChanged(range); });
    }
};

} // namespace sprint_timer::ui

#endif /* end of include guard: STATISTICSMEDIATOR_H_3CXN8PXD */
