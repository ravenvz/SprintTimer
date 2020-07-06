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
#ifndef MANUALSPRINTADDVIEW_H_ICBHYODJ
#define MANUALSPRINTADDVIEW_H_ICBHYODJ

#include "qt_gui/presentation/ManualSprintAddContract.h"
#include <QPushButton>

namespace sprint_timer::qt_gui {

class ManualSprintAddView : public ui::contracts::ManualSprintAddContract::View,
                            public QPushButton {
public:
    explicit ManualSprintAddView(
        ui::contracts::ManualSprintAddContract::Presenter& presenter,
        QWidget* parent = nullptr);

    ~ManualSprintAddView() override;

    void displayAddSprintDialog(const std::vector<entities::Task>& activeTasks,
                                dw::Weekday firstDayOfWeek,
                                std::chrono::minutes sprintDuration) override;

    void setInteractive(bool interactive) override;

private:
    ui::contracts::ManualSprintAddContract::Presenter& presenter;
};

} // namespace sprint_timer::qt_gui

#endif /* end of include guard: MANUALSPRINTADDVIEW_H_ICBHYODJ */
