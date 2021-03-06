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
#ifndef UNDODIALOG_H_WPYAGORZ
#define UNDODIALOG_H_WPYAGORZ

#include "qt_gui/dialogs/ConfirmationDialog.h"
#include <core/CommandInvoker.h>

namespace sprint_timer::ui::qt_gui {

class UndoDialog : public ConfirmationDialog {
public:
    explicit UndoDialog(CommandInvoker& commandInvoker,
                        QWidget* parent = nullptr);

    int exec() override;

    void accept() override;

private:
    CommandInvoker& commandInvoker;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: UNDODIALOG_H_WPYAGORZ */
