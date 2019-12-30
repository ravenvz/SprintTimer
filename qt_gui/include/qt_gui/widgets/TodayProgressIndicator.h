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
#ifndef TODAYPROGRESSINDICATOR_H_ZKLBW9AJ
#define TODAYPROGRESSINDICATOR_H_ZKLBW9AJ

#include "qt_gui/WorkScheduleWrapper.h"
#include <QAbstractItemModel>
#include <QLabel>
#include <QWidget>
#include <core/GoalProgress.h>

namespace sprint_timer::ui::qt_gui {

class TodayProgressIndicator : public QLabel {
public:
    TodayProgressIndicator(const QAbstractItemModel& todaySprintsModel,
                           const WorkScheduleWrapper& workScheduleWrapper,
                           QWidget* parent = nullptr);

private:
    GoalProgress progress;

    void update();
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TODAYPROGRESSINDICATOR_H_ZKLBW9AJ */

