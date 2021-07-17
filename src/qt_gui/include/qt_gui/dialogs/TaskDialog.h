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
#ifndef TASKDIALOG_H_J0SNZODV
#define TASKDIALOG_H_J0SNZODV

#include "qt_gui/dialogs/DisplayableDialog.h"
#include "core/use_cases/TaskDTO.h"

class QAbstractItemModel;
class QLineEdit;
class QSpinBox;

namespace sprint_timer::ui::qt_gui {

class TaskDialog : public DisplayableDialog {
public:
    explicit TaskDialog(QAbstractItemModel& tagModel,
                        QWidget* parent = nullptr);

protected:
    [[nodiscard]] use_cases::TaskDTO parseFormFields() const;

    void fillFormFields(const use_cases::TaskDTO& task);

    [[nodiscard]] bool nameIsEmpty() const;

    void markNameFieldRed();

private:
    QLineEdit* name;
    QSpinBox* cost;
    QLineEdit* tags;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TASKDIALOG_H_J0SNZODV */
