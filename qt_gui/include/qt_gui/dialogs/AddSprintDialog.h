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
#ifndef ADDSPRINTDIALOG_H
#define ADDSPRINTDIALOG_H

#include "qt_gui/delegates/SubmissionItemDelegate.h"
#include "qt_gui/dialogs/DisplayableDialog.h"
#include "qt_gui/presentation/RegisterSprintControl.h"
#include <QSpinBox>
#include <QTimeEdit>
#include <date_wrapper/date_wrapper.h>

namespace sprint_timer::ui::qt_gui {

class AddSprintDialog : public DisplayableDialog {

public:
    AddSprintDialog(contracts::RegisterSprintControl::Presenter& presenter,
                    QAbstractItemModel& taskModel,
                    dw::Weekday firstDayOfWeek,
                    std::chrono::minutes sprintDuration);

    ~AddSprintDialog() override;

private:
    std::chrono::minutes sprintDuration;
    QTimeEdit* startTime = std::make_unique<QTimeEdit>().release();
    QTimeEdit* finishTime = std::make_unique<QTimeEdit>().release();
    QSpinBox* sprintNumber = std::make_unique<QSpinBox>().release();
    QTimeEdit* lastChangedTime = startTime;
    SubmissionItemDelegate delegate;

    void adjustStartTime();

    void adjustFinishTime();

    void adjustTime();

    [[nodiscard]] std::chrono::seconds totalSprintTime() const;
};

} // namespace sprint_timer::ui::qt_gui

#endif // ADDSPRINTDIALOG_H
