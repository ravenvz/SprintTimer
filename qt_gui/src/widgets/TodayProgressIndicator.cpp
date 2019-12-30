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
#include "qt_gui/widgets/TodayProgressIndicator.h"

namespace {

constexpr char const* workgoalMetStyleSheet = "QLabel { color: green; }";
constexpr char const* overworkStyleSheet = "QLabel { color: red; }";
constexpr char const* underworkStyleSheet = "QLabel { color: black; }";

} // namespace

namespace sprint_timer::ui::qt_gui {

TodayProgressIndicator::TodayProgressIndicator(
    const QAbstractItemModel& todaySprintsModel_,
    const WorkScheduleWrapper& workScheduleWrapper_,
    QWidget* parent_)
    : QLabel{parent_}
{
    connect(&todaySprintsModel_,
            &QAbstractItemModel::modelReset,
            [this, &todaySprintsModel_]() {
                progress = GoalProgress{
                    GoalProgress::Estimated{progress.estimated()},
                    GoalProgress::Actual{todaySprintsModel_.rowCount()}};
                update();
            });
    connect(&workScheduleWrapper_,
            &WorkScheduleWrapper::workScheduleChanged,
            [this](const auto& workSchedule) {
                progress =
                    GoalProgress{GoalProgress::Estimated{workSchedule.goal(
                                     dw::current_date_local())},
                                 GoalProgress::Actual{progress.actual()}};
                update();
            });
}

void TodayProgressIndicator::update()
{
    const int estimated{progress.estimated()};
    const int actual{progress.actual()};

    if (estimated == 0) {
        hide();
        return;
    }

    show();
    setText(QString("Daily goal progress: %1/%2").arg(actual).arg(estimated));

    if (actual == estimated)
        setStyleSheet(workgoalMetStyleSheet);
    else if (actual > estimated)
        setStyleSheet(overworkStyleSheet);
    else
        setStyleSheet(underworkStyleSheet);
}

} // namespace sprint_timer::ui::qt_gui

