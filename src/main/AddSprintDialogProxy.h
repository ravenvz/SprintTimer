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
#include "DisplayableDialogLifestyleProxy.h"
#include <core/CompositionObserver.h>
#include <core/IConfig.h>
#include <qt_gui/dialogs/AddSprintDialog.h>

#ifndef ADDSPRINTDIALOGPROXY_H_QRM9YHDS
#define ADDSPRINTDIALOGPROXY_H_QRM9YHDS

namespace sprint_timer::compose {

class AddSprintDialogProxy : public DisplaybleDialogLifestyleProxy {
public:
    AddSprintDialogProxy(
        ui::contracts::RegisterSprintControl::Presenter& presenter_,
        QAbstractItemModel& taskModel_,
        IConfig& settings_,
        Observable& configChangedSignaller_)
        : presenter{presenter_}
        , taskModel{taskModel_}
        , settings{settings_}
        , configChangedWatcher{configChangedSignaller_,
                               [this]() { onConfigChanged(); }}
    {
    }

    std::unique_ptr<ui::qt_gui::DisplayableDialog> create() override
    {
        return std::make_unique<ui::qt_gui::AddSprintDialog>(
            presenter, taskModel, dw::Weekday::Sunday, sprintDuration);
    }

private:
    ui::contracts::RegisterSprintControl::Presenter& presenter;
    QAbstractItemModel& taskModel;
    IConfig& settings;
    CompositionObserver configChangedWatcher;
    std::chrono::minutes sprintDuration{settings.sprintDuration()};
    dw::Weekday firstDayOfWeek{settings.firstDayOfWeek()};

    void onConfigChanged()
    {
        sprintDuration = settings.sprintDuration();
        firstDayOfWeek = settings.firstDayOfWeek();
    }
};

} // namespace sprint_timer::compose

#endif /* end of include guard: ADDSPRINTDIALOGPROXY_H_QRM9YHDS */

