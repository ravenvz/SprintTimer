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

#ifndef SPRINT_TIMER_EXPORTDIALOG_H
#define SPRINT_TIMER_EXPORTDIALOG_H

#include <QDialog>
#include <memory>

namespace Ui {
class ExportDialog;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class ExportDialog : public QDialog {
    Q_OBJECT

public:
    struct ExportOptions {
        const std::string path;
        const char delimiter;
    };

    explicit ExportDialog(QWidget* parent = nullptr);
    ~ExportDialog() override;
    void accept() override;

private:
    std::unique_ptr<Ui::ExportDialog> ui;

signals:
    void exportConfirmed(const ExportOptions& options);
};

} // namespace sprint_timer::ui::qt_gui


#endif // SPRINT_TIMER_EXPORTDIALOG_H
