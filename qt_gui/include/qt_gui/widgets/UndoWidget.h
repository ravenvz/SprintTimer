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
#ifndef UNDOWIDGET_H_LMBNAZDR
#define UNDOWIDGET_H_LMBNAZDR

#include "qt_gui/presentation/UndoContract.h"
#include <QPushButton>

namespace sprint_timer::qt_gui {

class UndoWidget : public ui::contracts::UndoContract::View,
                   public QPushButton {
public:
    explicit UndoWidget(ui::contracts::UndoContract::Presenter& presenter,
                        QWidget* parent = nullptr);

    ~UndoWidget() override;

    void showConfirmationDialog(const std::string& message) override;

    void setInteractive(bool interactive) override;

private:
    ui::contracts::UndoContract::Presenter& presenter;
};

} // namespace sprint_timer::qt_gui

#endif /* end of include guard: UNDOWIDGET_H_LMBNAZDR */
