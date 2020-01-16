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
#include "include/qt_gui/WorkScheduleWrapper.h"

namespace sprint_timer::ui::qt_gui {

WorkScheduleWrapper::WorkScheduleWrapper(
    DatasyncRelay& datasyncRelay_,
    CommandHandler<use_cases::ChangeWorkScheduleCommand>&
        changeScheduleHandler_,
    QueryHandler<use_cases::WorkScheduleQuery, WorkSchedule>& scheduleHandler_,
    QObject* parent_)
    : QObject{parent_}
    , datasyncRelay{datasyncRelay_}
    , changeScheduleHandler{changeScheduleHandler_}
    , scheduleHandler{scheduleHandler_}
{
    connect(&datasyncRelay_,
            &DatasyncRelay::dataUpdateRequiered,
            this,
            &WorkScheduleWrapper::requestDataUpdate);
}

void WorkScheduleWrapper::requestDataUpdate()
{
    schedule = scheduleHandler.handle(use_cases::WorkScheduleQuery{});
    datasyncRelay.onSyncCompleted("WorkScheduleWrapper");
    emit workScheduleChanged(schedule);
}

void WorkScheduleWrapper::changeSchedule(
    const WorkSchedule& updatedWorkSchedule)
{
    changeScheduleHandler.handle(
        use_cases::ChangeWorkScheduleCommand{schedule, updatedWorkSchedule});
    schedule = updatedWorkSchedule;
    emit workScheduleChanged(schedule);
}

const WorkSchedule& WorkScheduleWrapper::workSchedule() const
{
    return schedule;
}

} // namespace sprint_timer::ui::qt_gui

