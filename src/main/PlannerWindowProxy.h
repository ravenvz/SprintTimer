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
#ifndef PLANNERWINDOWPROXY_H_3VTKAR0F
#define PLANNERWINDOWPROXY_H_3VTKAR0F

#include "DisplayableLifestyleProxy.h"
#include "qt_gui/models/PlannerModel.h"
#include "qt_gui/presentation/PlannerContract.h"
#include "qt_gui/widgets/PlannerWindow.h"

namespace sprint_timer::compose {

class PlannerWindowProxy : public DisplayableLifestyleProxy {
public:
    explicit PlannerWindowProxy(
        ui::contracts::PlannerContract::Presenter& presenter_);

private:
    ui::contracts::PlannerContract::Presenter& presenter;

    std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget> create() override;
};

inline PlannerWindowProxy::PlannerWindowProxy(
    ui::contracts::PlannerContract::Presenter& presenter_)
    : presenter{presenter_}
{
}

inline std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget>
PlannerWindowProxy::create()
{
    auto plannerModel = std::make_unique<ui::qt_gui::PlannerModel>();
    plannerModel->setPresenter(presenter);
    return std::make_unique<ui::qt_gui::PlannerWindow>(std::move(plannerModel));
}

} // namespace sprint_timer::compose

#endif /* end of include guard: PLANNERWINDOWPROXY_H_3VTKAR0F */
