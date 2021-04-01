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
#include "qt_gui/widgets/UndoWidget.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <memory>

namespace sprint_timer::qt_gui {

UndoWidget::UndoWidget(QWidget* parent_)
    : QPushButton{parent_}
{
    setText("Undo");
    connect(this, &QPushButton::clicked, [this]() {
        if (auto p = presenter(); p) {
            p.value()->onUndoRequested();
        }
    });
}

void UndoWidget::showConfirmationDialog(const std::string& message)
{
    QMessageBox messageBox;
    messageBox.setText(QString::fromStdString(message));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Abort);
    if (int ret = messageBox.exec(); ret == QMessageBox::Yes) {
        if (auto p = presenter(); p) {
            p.value()->onUndoConfirmed();
        }
    }
}

void UndoWidget::setInteractive(bool interactive) { setEnabled(interactive); }

} // namespace sprint_timer::qt_gui
