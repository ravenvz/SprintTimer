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
#ifndef CONFIRMATIONDIALOG_H
#define CONFIRMATIONDIALOG_H

#include <QDialog>
#include <memory>

namespace Ui {
class ConfirmationDialog;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class ConfirmationDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConfirmationDialog(QWidget* parent = nullptr);
    ~ConfirmationDialog() override;
    void setActionDescription(QString& description);

private:
    std::unique_ptr<Ui::ConfirmationDialog> ui;
};

} // namespace sprint_timer::ui::qt_gui


#endif // CONFIRMATIONDIALOG_H
