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
#include "qt_gui/dialogs/AddTaskDialog.h"
#include "qt_gui/dialogs/EditTaskDialog.h"
#include "qt_gui/models/PlannerModel.h"
#include "qt_gui/presentation/AddTaskControl.h"
#include "qt_gui/presentation/DateRangeSelectorContract.h"
#include "qt_gui/presentation/PlannerContract.h"
#include "qt_gui/presentation/TaskSelectionContext.h"
#include "qt_gui/widgets/PlannerWindow.h"

class QAbstractItemDelegate;

namespace sprint_timer::compose {

class PlannerWindowProxy : public DisplayableLifestyleProxy {
public:
    PlannerWindowProxy(ui::contracts::PlannerContract::Presenter& presenter_,
                       QAbstractItemModel& plannerModel_,
                       QAbstractItemDelegate& plannerItemDelegate_,
                       ui::qt_gui::Displayable& addTaskDialog_,
                       ui::qt_gui::Displayable& editTaskDialog_);

private:
    ui::contracts::PlannerContract::Presenter& presenter;
    QAbstractItemModel& plannerModel;
    QAbstractItemDelegate& plannerItemDelegate;
    ui::qt_gui::Displayable& addTaskDialog;
    ui::qt_gui::Displayable& editTaskDialog;

    auto create()
        -> std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget> override;
};

inline PlannerWindowProxy::PlannerWindowProxy(
    ui::contracts::PlannerContract::Presenter& presenter_,
    QAbstractItemModel& plannerModel_,
    QAbstractItemDelegate& plannerItemDelegate_,
    ui::qt_gui::Displayable& addTaskDialog_,
    ui::qt_gui::Displayable& editTaskDialog_)
    : presenter{presenter_}
    , plannerModel{plannerModel_}
    , plannerItemDelegate{plannerItemDelegate_}
    , addTaskDialog{addTaskDialog_}
    , editTaskDialog{editTaskDialog_}
{
}

inline auto PlannerWindowProxy::create()
    -> std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget>
{
    auto view = std::make_unique<ui::qt_gui::PlannerWindow>(
        plannerModel, plannerItemDelegate, addTaskDialog, editTaskDialog);
    view->setPresenter(presenter);
    return view;
}

} // namespace sprint_timer::compose

#endif /* end of include guard: PLANNERWINDOWPROXY_H_3VTKAR0F */
