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
#ifndef GOAL_PROGRESS_WIDGET_H
#define GOAL_PROGRESS_WIDGET_H

#include "qt_gui/widgets/ProgressView.h"
#include <core/IWorkingDaysReader.h>
#include <core/QueryInvoker.h>
#include <memory>

namespace sprint_timer::ui::qt_gui {

class ProgressView;

class ProgressMonitorWidget : public QWidget {
public:
    ProgressMonitorWidget(std::unique_ptr<ProgressView> dailyProgress,
                          std::unique_ptr<ProgressView> weeklyProgress,
                          std::unique_ptr<ProgressView> monthlyProgress,
                          QueryInvoker& queryInvoker,
                          IWorkingDaysReader& workingDaysReader,
                          QWidget* parent = 0);

    QSize sizeHint() const override;

    void requestWorkingDays() const;

private:
    QueryInvoker& queryInvoker;
    IWorkingDaysReader& workingDaysReader;
    std::vector<ProgressView*> progressViews;

    void onWorkingDaysRead(const WorkdayTracker& tracker) const;
};

} // namespace sprint_timer::ui::qt_gui


#endif // GOAL_PROGRESS_WIDGET_H
