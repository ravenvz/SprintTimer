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
#ifndef EDITTASKDIALOGPROXY_H_WZ7UTD03
#define EDITTASKDIALOGPROXY_H_WZ7UTD03

#include "DisplayableDialogLifestyleProxy.h"
#include "api/IConfig.h"
#include "qt_gui/dialogs/EditTaskDialog.h"

namespace sprint_timer::compose {

class EditTaskDialogProxy : public DisplaybleDialogLifestyleProxy {
public:
    EditTaskDialogProxy(
        ui::contracts::EditTaskDialogContract::Presenter& presenter,
        api::IConfig& settings);

    auto create() -> std::unique_ptr<ui::qt_gui::DisplayableDialog> override;

private:
    ui::contracts::EditTaskDialogContract::Presenter& presenter;
    api::IConfig& settings;
};

inline EditTaskDialogProxy::EditTaskDialogProxy(
    ui::contracts::EditTaskDialogContract::Presenter& presenter_,
    api::IConfig& settings_)
    : presenter{presenter_}
    , settings{settings_}
{
}

inline auto EditTaskDialogProxy::create()
    -> std::unique_ptr<ui::qt_gui::DisplayableDialog>
{
    auto view =
        std::make_unique<ui::qt_gui::EditTaskDialog>(settings.firstDayOfWeek());
    view->setPresenter(presenter);
    return view;
}

} // namespace sprint_timer::compose

#endif /* end of include guard: EDITTASKDIALOGPROXY_H_WZ7UTD03 */
