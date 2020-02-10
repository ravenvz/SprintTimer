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
#ifndef ADDSPRINTDIALOGPROXY_H_TXGPDOSL
#define ADDSPRINTDIALOGPROXY_H_TXGPDOSL

#include "ManagedStandaloneDisplayable.h"
#include <QComboBox>
#include <core/CommandHandler.h>
#include <core/IConfig.h>
#include <core/use_cases/register_sprint/RegisterSprintBulkCommand.h>
#include <qt_gui/delegates/SubmissionItemDelegate.h>
#include <qt_gui/dialogs/AddSprintDialog.h>
#include <qt_gui/models/TaskModel.h>

namespace sprint_timer::compose {

class AddSprintDialogProxy : public ManagedStandaloneDisplayable {
public:
    AddSprintDialogProxy(const IConfig& settings_,
                         CommandHandler<use_cases::RegisterSprintBulkCommand>&
                             registerSprintHandler_,
                         ui::qt_gui::TaskModel& taskModel_)
        : settings{settings_}
        , registerSprintHandler{registerSprintHandler_}
        , taskModel{taskModel_}
    {
    }

private:
    const IConfig& settings;
    CommandHandler<use_cases::RegisterSprintBulkCommand>& registerSprintHandler;
    ui::qt_gui::TaskModel& taskModel;

    std::unique_ptr<StandaloneDisplayable> create() override
    {
        auto taskSelector = std::make_unique<QComboBox>();
        taskSelector->setModel(&taskModel);
        taskSelector->setItemDelegate(
            std::make_unique<ui::qt_gui::SubmissionItemDelegate>().release());
        return std::make_unique<ui::qt_gui::AddSprintDialog>(
            settings.sprintDuration(),
            settings.firstDayOfWeek(),
            std::move(taskSelector),
            registerSprintHandler);
    }
};

} // namespace sprint_timer::compose

#endif /* end of include guard: ADDSPRINTDIALOGPROXY_H_TXGPDOSL */
