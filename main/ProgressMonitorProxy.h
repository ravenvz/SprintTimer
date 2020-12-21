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

#include "DisplayableLifestyleProxy.h"
#include <qt_gui/StandaloneDisplayable.h>
#include <qt_gui/dialogs/WorkScheduleEditor.h>
#include <qt_gui/presentation/BasePresenter.h>
#include <qt_gui/presentation/WorkScheduleEditorPresenter.h>
#include <qt_gui/widgets/DialogLaunchButton.h>
#include <qt_gui/widgets/ProgressMonitorWidget.h>
#include <qt_gui/widgets/ProgressWidget.h>

namespace sprint_timer::compose {

class ProgressMonitorProxy : public DisplayableLifestyleProxy {
public:
    ProgressMonitorProxy(
        ui::BasePresenter<ui::contracts::DailyProgress::View>& dailyPresenter_,
        ui::BasePresenter<ui::contracts::DailyProgress::View>& weeklyPresenter_,
        ui::BasePresenter<ui::contracts::DailyProgress::View>&
            monthlyPresenter_,
        ui::contracts::WorkScheduleEditor::Presenter& scheduleEditorPresenter_);

    std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget> create() override;

private:
    ui::BasePresenter<ui::contracts::DailyProgress::View>& dailyPresenter;
    ui::BasePresenter<ui::contracts::DailyProgress::View>& weeklyPresenter;
    ui::BasePresenter<ui::contracts::DailyProgress::View>& monthlyPresenter;
    ui::contracts::WorkScheduleEditor::Presenter& scheduleEditorPresenter;
    std::unique_ptr<QDialog> scheduleEditor;
};

inline ProgressMonitorProxy::ProgressMonitorProxy(
    ui::BasePresenter<ui::contracts::DailyProgress::View>& dailyPresenter_,
    ui::BasePresenter<ui::contracts::DailyProgress::View>& weeklyPresenter_,
    ui::BasePresenter<ui::contracts::DailyProgress::View>& monthlyPresenter_,
    ui::contracts::WorkScheduleEditor::Presenter& scheduleEditorPresenter_)
    : dailyPresenter{dailyPresenter_}
    , weeklyPresenter{weeklyPresenter_}
    , monthlyPresenter{monthlyPresenter_}
    , scheduleEditorPresenter{scheduleEditorPresenter_}
{
}

inline std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget>
ProgressMonitorProxy::create()
{
    using namespace ui::qt_gui;
    auto dailyProgress =
        std::make_unique<ProgressWidget>(dailyPresenter,
                                         ProgressWidget::Rows{3},
                                         ProgressWidget::Columns{10},
                                         ProgressWidget::GaugeSize{0.8});
    dailyProgress->setLegendTitle("Last 30 days");
    dailyProgress->setLegendAverageCaption("Average per day");
    scheduleEditor =
        std::make_unique<WorkScheduleEditor>(scheduleEditorPresenter);
    auto configureWorkdaysButton =
        std::make_unique<DialogLaunchButton>(*scheduleEditor, "Configure");
    dailyProgress->addLegendRow("Workdays", configureWorkdaysButton.release());

    auto weeklyProgress =
        std::make_unique<ProgressWidget>(weeklyPresenter,
                                         ProgressWidget::Rows{3},
                                         ProgressWidget::Columns{4},
                                         ProgressWidget::GaugeSize{0.8});
    weeklyProgress->setLegendTitle("Last 12 weeks");
    weeklyProgress->setLegendAverageCaption("Average per week:");

    auto monthlyProgress =
        std::make_unique<ProgressWidget>(monthlyPresenter,
                                         ProgressWidget::Rows{3},
                                         ProgressWidget::Columns{4},
                                         ProgressWidget::GaugeSize{0.8});
    monthlyProgress->setLegendTitle("Last 12 months");
    monthlyProgress->setLegendAverageCaption("Average per month:");

    // QPointer<StandaloneDisplayableWidget> ptr =
    //     new ProgressMonitorWidget{std::move(dailyProgress),
    //                                           std::move(weeklyProgress),
    //                                           std::move(monthlyProgress)};
    // return ptr;
    return std::make_unique<ProgressMonitorWidget>(std::move(dailyProgress),
                                                   std::move(weeklyProgress),
                                                   std::move(monthlyProgress));
    // return ptr;
    // return QPointer(new ProgressMonitorWidget>(
    //     std::move(dailyProgress),
    //     std::move(weeklyProgress),
    //     std::move(monthlyProgress));
}

} // namespace sprint_timer::compose

#endif /* end of include guard: PROGRESSMONITORPROXY_H_LTB9EOMJ */

