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
#include "include/qt_gui/models/WorkdayTrackerModel.h"
#include <core/use_cases/ChangeWorkingDays.h>

namespace sprint_timer::ui::qt_gui {

WorkdayTrackerModel::WorkdayTrackerModel(IWorkingDaysWriter& workingDaysWriter_,
                                         IWorkingDaysReader& workingDaysReader_,
                                         CommandInvoker& commandInvoker_,
                                         QueryInvoker& queryInvoker_,
                                         QObject* parent)
    : QObject{parent}
    , workingDaysWriter{workingDaysWriter_}
    , workingDaysReader{workingDaysReader_}
    , commandInvoker{commandInvoker_}
    , queryInvoker{queryInvoker_}
{
}

// TODO probably should just call it when constructing
void WorkdayTrackerModel::requestDataUpdate()
{
    using use_cases::RequestWorkingDays;
    queryInvoker.execute(std::make_unique<RequestWorkingDays>(
        workingDaysReader, [this](WorkdayTracker&& newTracker) {
            tracker = std::move(newTracker);
            emit workdaysChanged(tracker);
        }));
}

// TODO do we still need this overload at all?
void WorkdayTrackerModel::changeWorkdayData(
    const WeekSchedule& updatedSchedule,
    const std::vector<WorkdayTracker::DayData>& exceptionalDays)
{
    using sprint_timer::use_cases::ChangeWorkingDays;
    WorkdayTracker updatedTracker{tracker};
    updatedTracker.addWeekSchedule(dw::current_date_local(), updatedSchedule);
    for (const auto& [date, goal] : exceptionalDays)
        updatedTracker.addExceptionalDay(date, goal);
    commandInvoker.executeCommand(std::make_unique<ChangeWorkingDays>(
        workingDaysWriter, tracker, updatedTracker));
    tracker = updatedTracker;
    emit workdaysChanged(tracker);
}

void WorkdayTrackerModel::changeWorkdayData(
    const WorkdayTracker& updatedTracker)
{
    using sprint_timer::use_cases::ChangeWorkingDays;
    commandInvoker.executeCommand(std::make_unique<ChangeWorkingDays>(
        workingDaysWriter, tracker, updatedTracker));
    tracker = updatedTracker;
    emit workdaysChanged(tracker);
}

const WorkdayTracker& WorkdayTrackerModel::workdayTracker() const
{
    return tracker;
}

} // namespace sprint_timer::ui::qt_gui

