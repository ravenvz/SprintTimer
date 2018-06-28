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


#include "core/Distribution.h"
#include "dialogs/WorkdaysDialog.h"
#include "widgets/DataWidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <core/IConfig.h>
#include <core/ICoreService.h>
#include <memory>

namespace sprint_timer::ui::qt_gui {

class ProgressView;


class GoalProgressWindow : public DataWidget {
public:
    GoalProgressWindow(IConfig& applicationSettings,
                       ICoreService& coreService,
                       QWidget* parent = 0);
    virtual void synchronize() override;

    virtual QSize sizeHint() const override;

private:
    IConfig& applicationSettings;
    ICoreService& coreService;
    QGridLayout* layout;
    ProgressView* dailyProgress;
    ProgressView* weeklyProgress;
    ProgressView* monthlyProgress;
    QPushButton* configureWorkdaysButton;
    std::unique_ptr<WorkdaysDialog> workdaysDialog;
    const QSize expectedSize{1225, 650};

    void synchronizeDailyData();

    void synchronizeWeeklyData();

    void synchronizeMonthlyData();

    void onDailyDataReceived(const Distribution<int>& distribution);

    void onWeeklyDataReceived(const Distribution<int>& distribution);

    void onMonthlyDataReceived(const Distribution<int>& distribution);

private slots:
    void launchWorkdaysConfigurationDialog();
};

} // namespace sprint_timer::ui::qt_gui



#endif // GOAL_PROGRESS_WIDGET_H
