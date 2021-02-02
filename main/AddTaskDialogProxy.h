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
#ifndef ADDTASKDIALOGPROXY_H_JMC1VRX7
#define ADDTASKDIALOGPROXY_H_JMC1VRX7

#include "DisplayableDialogLifestyleProxy.h"
#include <qt_gui/dialogs/AddTaskDialog.h>
#include <qt_gui/presentation/AddTaskControl.h>

namespace sprint_timer::compose {

class AddTaskDialogProxy : public DisplaybleDialogLifestyleProxy {
public:
    AddTaskDialogProxy(
        sprint_timer::ui::contracts::AddTaskControl::Presenter& presenter,
        QAbstractItemModel& tagModel);

private:
    sprint_timer::ui::contracts::AddTaskControl::Presenter& presenter;
    QAbstractItemModel& tagModel;

    std::unique_ptr<ui::qt_gui::DisplayableDialog> create() override;
};

inline AddTaskDialogProxy::AddTaskDialogProxy(
    sprint_timer::ui::contracts::AddTaskControl::Presenter& presenter_,
    QAbstractItemModel& tagModel_)
    : presenter{presenter_}
    , tagModel{tagModel_}
{
}

inline std::unique_ptr<ui::qt_gui::DisplayableDialog>
AddTaskDialogProxy::create()
{
    return std::make_unique<ui::qt_gui::AddTaskDialog>(tagModel, presenter);
}

} // namespace sprint_timer::compose

#endif /* end of include guard: ADDTASKDIALOGPROXY_H_JMC1VRX7 */
