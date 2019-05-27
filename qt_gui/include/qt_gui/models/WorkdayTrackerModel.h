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
#ifndef WORKDAYTRACKERMODEL_H_4DZKR8HV
#define WORKDAYTRACKERMODEL_H_4DZKR8HV

#include <core/CommandInvoker.h>
#include <core/IWorkingDaysReader.h>
#include <core/IWorkingDaysWriter.h>
#include <core/QueryInvoker.h>
#include <core/WorkdayTracker.h>
#include <core/use_cases/RequestWorkingDays.h>
#include <QObject>

#ifdef _MSC_VER
#include "qt_gui/WinExport.h"
#endif // _MSC_VER

namespace sprint_timer::ui::qt_gui {

#ifdef _MSC_VER
class GLIB_EXPORT WorkdayTrackerModel : public QObject {
#else
class WorkdayTrackerModel : public QObject {
#endif // _MSC_VER

    Q_OBJECT

public:
    WorkdayTrackerModel(IWorkingDaysWriter& workingDaysWriter_,
                        IWorkingDaysReader& workingDaysReader_,
                        CommandInvoker& commandInvoker_,
                        QueryInvoker& queryInvoker_,
                        QObject* parent = nullptr);

    void requestDataUpdate();

    /* Adds updated schedule keeping previous ones and replaces exceptional days
     * with the given ones. */
    void changeWorkdayData(
        const WeekSchedule& updatedSchedule,
        const std::vector<WorkdayTracker::DayData>& exceptionalDays);

    void changeWorkdayData(const WorkdayTracker& updatedTracker);

    const WorkdayTracker& workdayTracker() const;

signals:
    void workdaysChanged(const WorkdayTracker&);

private:
    IWorkingDaysWriter& workingDaysWriter;
    IWorkingDaysReader& workingDaysReader;
    CommandInvoker& commandInvoker;
    QueryInvoker& queryInvoker;
    WorkdayTracker tracker;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: WORKDAYTRACKERMODEL_H_4DZKR8HV */

