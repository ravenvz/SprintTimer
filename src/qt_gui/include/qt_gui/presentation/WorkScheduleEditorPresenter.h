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
#ifndef WORKSCHEDULEEDITORPRESENTER_H_B6HTHBSK
#define WORKSCHEDULEEDITORPRESENTER_H_B6HTHBSK

#include "api/com_query/CommandHandler.h"
#include "api/com_query/QueryHandler.h"
#include "api/requests/ChangeWorkScheduleCommand.h"
#include "api/requests/WorkScheduleQuery.h"
#include "qt_gui/presentation/WorkScheduleEditor.h"

namespace sprint_timer::ui {

class WorkScheduleEditorPresenter
    : public contracts::WorkScheduleEditor::Presenter {
public:
    using work_schedule_handler_t = asp::QueryHandler<api::WorkScheduleQuery>;
    using change_schedule_handler_t =
        asp::CommandHandler<api::ChangeWorkScheduleCommand>;
    WorkScheduleEditorPresenter(
        work_schedule_handler_t& workScheduleHandler,
        change_schedule_handler_t& changeWorkScheduleHandler,
        dw::Weekday firstDayOfWeek);

    void onAddExceptionalRequested() override;

    void onExceptionalDaysAdded(dw::Date startDate,
                                int32_t numDays,
                                int32_t sprintsPerDay) override;

    void onExceptionalDayRemoved(dw::Date date) override;

    void onWeekScheduleAdded(std::vector<uint8_t>&& schedule,
                             dw::Date startDate) override;

    void onWeekScheduleRemoved(dw::Date scheduleStartDate) override;

    void onScheduleChangeConfirmed() override;

    void onRevertChanges() override;

private:
    std::reference_wrapper<work_schedule_handler_t> workScheduleHandler;
    std::reference_wrapper<change_schedule_handler_t> changeWorkScheduleHandler;
    dw::Weekday firstDayOfWeek;
    WorkSchedule bufferedSchedule;

    void updateViewImpl() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: WORKSCHEDULEEDITORPRESENTER_H_B6HTHBSK */
