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
#ifndef WORKDAYSDIALOG_H_NVZH9CRG
#define WORKDAYSDIALOG_H_NVZH9CRG

#include "qt_gui/WorkdaysChangeListener.h"
#include "qt_gui/dialogs/AddExceptionalDayDialog.h"
#include "qt_gui/models/WorkdayTrackerModel.h"
#include <QAbstractListModel>
#include <QDialog>
#include <memory>

namespace Ui {
class WorkdaysDialog;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class WorkdaysDialog : public QDialog, public WorkdaysChangeListener {
    Q_OBJECT

public:
    explicit WorkdaysDialog(AddExceptionalDayDialog& addExcDayDialog,
                            QAbstractItemModel& exceptionalDaysModel,
                            WorkdayTrackerModel& workdaysModel,
                            QAbstractItemModel& scheduleModel,
                            QDialog* parent = nullptr);

    ~WorkdaysDialog() override;

    void accept() override;

    void reject() override;

private:
    std::unique_ptr<Ui::WorkdaysDialog> ui;
    AddExceptionalDayDialog& pickDateDialog;
    WorkdayTrackerModel& workdaysModel;
    WorkdayTracker candidateTracker;

    void onWorkdayTrackerChanged(const WorkdayTracker& updatedTracker) override;

    void updateWorkdaysView(const WorkdayTracker& updatedTracker);

    void updateSchedulesView(const WorkdayTracker& updatedTracker);

    void initializeDayBoxes(const WeekSchedule& schedule);

    WeekSchedule pollSchedule() const;

    void addExceptionalDay();

    void addSchedule();

    void onExceptionalDayRemovedFromModel(const QModelIndex&, int first, int);

    void onScheduleRemovedFromModel(const QModelIndex&, int first, int);
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: WORKDAYSDIALOG_H_NVZH9CRG */
