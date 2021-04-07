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
#ifndef WORKSCHEDULEEDITOR_H_UCRXQH70
#define WORKSCHEDULEEDITOR_H_UCRXQH70

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include <core/WorkSchedule.h>
#include <date_wrapper/date_wrapper.h>

namespace sprint_timer::ui::contracts::WorkScheduleEditor {

using DayAndGoal = std::pair<unsigned, unsigned>;

using RoasterRow = std::pair<dw::Date, std::string>;

using ExceptionalDay = std::pair<dw::Date, uint32_t>;

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
    virtual void onAddExceptionalRequested() = 0;

    virtual void onExceptionalDaysAdded(dw::Date startDate,
                                        int32_t numDays,
                                        int32_t sprintsPerDay) = 0;

    virtual void onExceptionalDayRemoved(dw::Date date) = 0;

    virtual void onWeekScheduleAdded(std::vector<uint8_t>&& schedule,
                                     dw::Date startDate) = 0;

    virtual void onWeekScheduleRemoved(dw::Date scheduleStartDate) = 0;

    virtual void onScheduleChangeConfirmed() = 0;

    virtual void onRevertChanges() = 0;
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual void
    displayCurrentWeekSchedule(const std::vector<DayAndGoal>& weekSchedule) = 0;

    virtual void displayRoaster(const std::vector<RoasterRow>& roaster) = 0;

    virtual void displayExceptionalDays(
        const std::vector<WorkSchedule::DateGoal>& exceptionalDays) = 0;

    virtual void displayAddExceptionalDaysDialog(dw::Weekday firstDayOfWeek,
                                                 dw::Date preselectedDate) = 0;

    View() = default;
    View(const View& other) = delete;
    View& operator=(const View& other) = delete;
    View(View&& other) noexcept = delete;
    View& operator=(View&& other) noexcept = delete;
};

} // namespace sprint_timer::ui::contracts::WorkScheduleEditor

#endif /* end of include guard: WORKSCHEDULEEDITOR_H_UCRXQH70 */
