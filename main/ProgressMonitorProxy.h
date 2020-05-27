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
#ifndef PROGRESSMONITORPROXY_H_LTB9EOMJ
#define PROGRESSMONITORPROXY_H_LTB9EOMJ

#include "ManagedDisplayable.h"
#include "ManagedStandaloneDisplayable.h"
#include <qt_gui/presentation/BasePresenter.h>
#include <qt_gui/widgets/DialogLaunchButton.h>
#include <qt_gui/widgets/ProgressMonitorWidget.h>
#include <qt_gui/widgets/ProgressWidget.h>

namespace sprint_timer::compose {

class ProgressMonitorProxy : public ManagedStandaloneDisplayable {
public:
    ProgressMonitorProxy(
        ui::BasePresenter<ui::contracts::DailyProgress::View>& dailyPresenter_,
        ui::BasePresenter<ui::contracts::DailyProgress::View>& weeklyPresenter_,
        ui::BasePresenter<ui::contracts::DailyProgress::View>&
            monthlyPresenter_,
        QDialog& workdaysDialog_)
        : dailyPresenter{dailyPresenter_}
        , weeklyPresenter{weeklyPresenter_}
        , monthlyPresenter{monthlyPresenter_}
        , workdaysDialog{workdaysDialog_}
    {
    }

    std::unique_ptr<StandaloneDisplayable> create() override
    {
        auto dailyProgress = std::make_unique<ui::qt_gui::ProgressWidget>(
            dailyPresenter,
            ui::qt_gui::ProgressWidget::Rows{3},
            ui::qt_gui::ProgressWidget::Columns{10},
            ui::qt_gui::ProgressWidget::GaugeSize{0.8});
        dailyProgress->setLegendTitle("Last 30 days");
        dailyProgress->setLegendAverageCaption("Average per day");
        auto configureWorkdaysButton =
            std::make_unique<ui::qt_gui::DialogLaunchButton>(workdaysDialog,
                                                             "Configure");
        dailyProgress->addLegendRow("Workdays",
                                    configureWorkdaysButton.release());

        auto weeklyProgress = std::make_unique<ui::qt_gui::ProgressWidget>(
            weeklyPresenter,
            ui::qt_gui::ProgressWidget::Rows{3},
            ui::qt_gui::ProgressWidget::Columns{4},
            ui::qt_gui::ProgressWidget::GaugeSize{0.8});
        weeklyProgress->setLegendTitle("Last 12 weeks");
        weeklyProgress->setLegendAverageCaption("Average per week:");

        auto monthlyProgress = std::make_unique<ui::qt_gui::ProgressWidget>(
            monthlyPresenter,
            ui::qt_gui::ProgressWidget::Rows{3},
            ui::qt_gui::ProgressWidget::Columns{4},
            ui::qt_gui::ProgressWidget::GaugeSize{0.8});
        monthlyProgress->setLegendTitle("Last 12 months");
        monthlyProgress->setLegendAverageCaption("Average per month:");

        return std::make_unique<ui::qt_gui::ProgressMonitorWidget>(
            std::move(dailyProgress),
            std::move(weeklyProgress),
            std::move(monthlyProgress));
    }

private:
    ui::BasePresenter<ui::contracts::DailyProgress::View>& dailyPresenter;
    ui::BasePresenter<ui::contracts::DailyProgress::View>& weeklyPresenter;
    ui::BasePresenter<ui::contracts::DailyProgress::View>& monthlyPresenter;
    QDialog& workdaysDialog;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: PROGRESSMONITORPROXY_H_LTB9EOMJ */

