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
#ifndef ADDSPRINTDIALOG_H
#define ADDSPRINTDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QtWidgets/QCalendarWidget>
#include <core/CommandHandler.h>
#include <core/IConfig.h>
#include <core/SprintStorageWriter.h>
#include <core/use_cases/register_sprint/RegisterSprintBulkCommand.h>
#include <memory>

namespace Ui {
class AddSprintDialog;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class AddSprintDialog : public QDialog {
    Q_OBJECT

public:
    AddSprintDialog(const IConfig& applicationSettings,
                    std::unique_ptr<QComboBox> taskSelector,
                    CommandHandler<use_cases::RegisterSprintBulkCommand>&
                        registerSprintBulkHandler,
                    QDialog* parent = nullptr);

    ~AddSprintDialog() override;

    void accept() override;

private slots:
    void adjustFinishTime();
    void adjustStartTime();

private:
    std::unique_ptr<Ui::AddSprintDialog> ui;
    std::unique_ptr<QCalendarWidget> datePicker;
    const IConfig& applicationSettings;
    QComboBox* taskSelector;
    CommandHandler<use_cases::RegisterSprintBulkCommand>&
        registerSprintBulkHandler;

    std::chrono::seconds totalSprintLength() const;

    void resetDataFields();
};

} // namespace sprint_timer::ui::qt_gui

#endif // ADDSPRINTDIALOG_H
