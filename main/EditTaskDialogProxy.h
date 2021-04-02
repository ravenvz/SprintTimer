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
#ifndef EDITTASKDIALOGPROXY_H_WZ7UTD03
#define EDITTASKDIALOGPROXY_H_WZ7UTD03

#include "DisplayableDialogLifestyleProxy.h"
#include <qt_gui/dialogs/EditTaskDialog.h>

namespace sprint_timer::compose {

class EditTaskDialogProxy : public DisplaybleDialogLifestyleProxy {
public:
    EditTaskDialogProxy(QAbstractItemModel& tagModel,
                        QAbstractItemModel& taskModel,
                        const ui::TaskSelectionContext& taskSelectionContext);

    std::unique_ptr<ui::qt_gui::DisplayableDialog> create() override;

private:
    QAbstractItemModel& tagModel;
    QAbstractItemModel& taskModel;
    const ui::TaskSelectionContext& taskSelectionContext;
};

EditTaskDialogProxy::EditTaskDialogProxy(
    QAbstractItemModel& tagModel_,
    QAbstractItemModel& taskModel_,
    const ui::TaskSelectionContext& taskSelectionContext_)
    : tagModel{tagModel_}
    , taskModel{taskModel_}
    , taskSelectionContext{taskSelectionContext_}
{
}

inline std::unique_ptr<ui::qt_gui::DisplayableDialog>
EditTaskDialogProxy::create()
{
    return std::make_unique<ui::qt_gui::EditTaskDialog>(
        tagModel, taskModel, taskSelectionContext);
}

} // namespace sprint_timer::compose

#endif /* end of include guard: EDITTASKDIALOGPROXY_H_WZ7UTD03 */
