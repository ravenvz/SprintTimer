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

#include <QDialog>
#include <QSpinBox>
#include <QTimeEdit>
#include <core/entities/Sprint.h>
#include <core/entities/Task.h>
#include <memory>

namespace sprint_timer::ui::qt_gui {

class AddSprintDialog : public QDialog {

public:
    AddSprintDialog(const std::vector<entities::Task>& tasks,
                    std::vector<entities::Sprint>& addedSprints,
                    dw::Weekday firstDayOfWeek,
                    std::chrono::minutes sprintDuration,
                    QDialog* parent = nullptr);

    ~AddSprintDialog() override;

    void accept() override;

private:
    std::chrono::minutes sprintDuration;
    QTimeEdit* startTime = std::make_unique<QTimeEdit>().release();
    QTimeEdit* finishTime = std::make_unique<QTimeEdit>().release();
    QSpinBox* sprintNumber = std::make_unique<QSpinBox>().release();
    QTimeEdit* lastChangedTime = startTime;

    void adjustStartTime(const QDateTime&);

    void adjustFinishTime(const QDateTime&);

    void adjustTime();
};

} // namespace sprint_timer::ui::qt_gui

#endif // ADDSPRINTDIALOG_H
