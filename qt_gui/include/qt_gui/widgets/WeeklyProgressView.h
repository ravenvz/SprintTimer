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
#ifndef WEEKLYPROGRESSVIEW_H_BF6U7N1X
#define WEEKLYPROGRESSVIEW_H_BF6U7N1X

#include "qt_gui/widgets/ProgressView.h"
#include <core/IConfig.h>
#include <core/ISprintDistributionReader.h>
#include <core/QueryInvoker.h>

namespace sprint_timer::ui::qt_gui {

class WeeklyProgressView : public ProgressView {
public:
    WeeklyProgressView(IConfig& applicationSettings,
                       QueryInvoker& queryInvoker,
                       ISprintDistributionReader& weeklyDistributionReader,
                       QWidget* parent = nullptr);

    void synchronize() override;

private:
    IConfig& applicationSettings;
    const FirstDayOfWeek firstDayOfWeek{applicationSettings.firstDayOfWeek()};
    QueryInvoker& queryInvoker;
    ISprintDistributionReader& distributionReader;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: WEEKLYPROGRESSVIEW_H_BF6U7N1X */
