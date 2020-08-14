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
#ifndef WORKSCHEDULEEDITOR_H_N2VONPYL
#define WORKSCHEDULEEDITOR_H_N2VONPYL

#include "qt_gui/presentation/WorkScheduleEditor.h"
#include <QAbstractListModel>
#include <QDialog>
#include <memory>

namespace Ui {
class WorkScheduleEditor;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class WorkScheduleEditor : public contracts::WorkScheduleEditor::View,
                           public QDialog {

public:
    explicit WorkScheduleEditor(
        contracts::WorkScheduleEditor::Presenter& presenter,
        QDialog* parent = nullptr);

    ~WorkScheduleEditor() override;

    void accept() override;

    void reject() override;

    void displayCurrentWeekSchedule(
        const std::vector<contracts::WorkScheduleEditor::DayAndGoal>&
            weekSchedule) override;

    void displayRoaster(
        const std::vector<contracts::WorkScheduleEditor::RoasterRow>& roaster)
        override;

    void displayExceptionalDays(
        const std::vector<WorkSchedule::DateGoal>& exceptionalDays) override;

    void displayAddExceptionalDaysDialog(dw::Weekday firstDayOfWeek,
                                         dw::Date preselectedDate) override;

private:
    std::unique_ptr<Ui::WorkScheduleEditor> ui;
    contracts::WorkScheduleEditor::Presenter& presenter;
    std::unique_ptr<QAbstractItemModel> exceptionalDaysModel;
    std::unique_ptr<QAbstractItemModel> roasterModel;
    std::unique_ptr<QAbstractItemModel> roasterBufferModel;

    void addSchedule();

    std::vector<uint8_t> pollSchedule() const;

    void onExcDayAboutToBeRemoved(const QModelIndex&, int first, int);

    void onScheduleRemovedFromModel(const QModelIndex&, int first, int);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: WORKSCHEDULEEDITOR_H_N2VONPYL */

