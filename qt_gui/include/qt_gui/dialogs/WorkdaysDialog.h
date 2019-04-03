/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#include "qt_gui/dialogs/AddExceptionalDayDialog.h"
#include "qt_gui/models/ExtraDayModel.h"
#include <QDialog>
#include <core/CommandInvoker.h>
#include <core/IConfig.h>
#include <core/IWorkingDaysWriter.h>
#include <core/WorkdayTracker.h>
#include <memory>

namespace Ui {
class WorkdaysDialog;
} // namespace Ui

namespace {

class State;

} // namespace

namespace sprint_timer::ui::qt_gui {

class WorkdaysDialog : public QDialog {
    Q_OBJECT

public:
    explicit WorkdaysDialog(AddExceptionalDayDialog& addExcDayDialog,
                            CommandInvoker& commandInvoker,
                            IWorkingDaysWriter& workingDaysWriter,
                            ExtraDayModel& holidayModel,
                            ExtraDayModel& workdayModel,
                            QDialog* parent = nullptr);

    ~WorkdaysDialog() override;

    void accept() override;

    void setWorkdayTracker(const WorkdayTracker& workdayTracker);

    WorkdayTracker workdayTracker() const;

private:
    std::unique_ptr<Ui::WorkdaysDialog> ui;
    AddExceptionalDayDialog& pickDateDialog;
    CommandInvoker& commandInvoker;
    IWorkingDaysWriter& workingDaysWriter;
    ExtraDayModel& holidayModel;
    ExtraDayModel& workdayModel;
    WorkdayTracker tracker;

    void initializeDayBoxes();

    utils::WeekdaySelection pollWorkdaysCode() const;

    void addHolidays();

    void addWorkdays();

    void
    onHolidayRemoveRequested(const QModelIndex& parent, int row, int count);

    void
    onWorkdayRemoveRequested(const QModelIndex& parent, int row, int count);
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: WORKDAYSDIALOG_H_NVZH9CRG */
