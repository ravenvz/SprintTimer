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
#ifndef WORKSCHEDULEWRAPPER_H_8VTK5QM1
#define WORKSCHEDULEWRAPPER_H_8VTK5QM1

#include "qt_gui/DatasyncRelay.h"
#include <QObject>
#include <core/CommandInvoker.h>
#include <core/IWorkingDaysStorage.h>
#include <core/QueryInvoker.h>
#include <core/WorkSchedule.h>
#include <core/use_cases/RequestWorkingDays.h>

#ifdef _MSC_VER
#include "qt_gui/WinExport.h"
#endif // _MSC_VER

namespace sprint_timer::ui::qt_gui {

#ifdef _MSC_VER
class GLIB_EXPORT WorkScheduleWrapper : public QObject {
#else
class WorkScheduleWrapper : public QObject {
#endif // _MSC_VER

    Q_OBJECT

public:
    WorkScheduleWrapper(IWorkingDaysStorage& workingDaysStorage,
                        CommandInvoker& commandInvoker,
                        QueryInvoker& queryInvoker,
                        DatasyncRelay& datasyncRelay,
                        QObject* parent = nullptr);

    void requestDataUpdate();

    void changeSchedule(const WorkSchedule& updatedWorkSchedule);

    const WorkSchedule& workSchedule() const;

signals:
    void workScheduleChanged(const WorkSchedule&);

private:
    IWorkingDaysStorage& workingDaysStorage;
    CommandInvoker& commandInvoker;
    QueryInvoker& queryInvoker;
    DatasyncRelay& datasyncRelay;
    WorkSchedule schedule;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: WORKSCHEDULEWRAPPER_H_8VTK5QM1 */
