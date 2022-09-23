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

#include "api/dtos/TaskDTO.h"
#include "qt_gui/dialogs/DisplayableDialog.h"
#include "qt_gui/presentation/DateRangeSelectorContract.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include <QStringListModel>

class QAbstractItemModel;
class QLineEdit;
class QSpinBox;
class QDateTimeEdit;
class QTextEdit;
class QGroupBox;
class QComboBox;

namespace sprint_timer::ui::qt_gui {

class TaskDialog : public DisplayableDialog {
public:
    explicit TaskDialog(dw::Weekday firstDayOfWeek, QWidget* parent = nullptr);

protected:
    [[nodiscard]] auto parseFormFields() const -> api::TaskDTO;

    auto fillFormFields(const api::TaskDTO& task) -> void;

    auto fillTagField(std::span<const std::string> tags) -> void;

    [[nodiscard]] auto nameIsEmpty() const -> bool;

    auto markNameFieldRed() -> void;

private:
    dw::Weekday firstDayOfWeek;
    QLineEdit* name;
    QSpinBox* cost;
    QLineEdit* tagsField;
    QStringListModel typesModel;
    QStringListModel tagModel;
    QComboBox* typesBox;
    QDateTimeEdit* start;
    QGroupBox* dueFrame;
    QDateTimeEdit* due;
    QGroupBox* reminderFrame;
    QDateTimeEdit* reminder;
    QLineEdit* recurrence;
    QTextEdit* notes;
    utils::DateTimeConverter dateTimeConverter;

    auto maybeDateTime(QDateTimeEdit* widget) const
        -> std::optional<dw::DateTime>;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TASKDIALOG_H_J0SNZODV */
