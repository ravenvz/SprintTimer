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
#include <QDate>
#include <QDialog>
#include <memory>

#ifndef ADDEXCEPTIONALDAYDIALOG_H_4KBYLJOJ
#define ADDEXCEPTIONALDAYDIALOG_H_4KBYLJOJ

namespace Ui {

class AddExceptionalDayDialog;

} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class AddExceptionalDayDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddExceptionalDayDialog(QDialog* parent = nullptr);

    ~AddExceptionalDayDialog() override;

    void accept() override;

    QDate startDate() const;

    int numDays() const;

    void setStartDate(const QDate& date);

    void setNumDays(int numDays);

signals:
    void onDateSelected(const QDate& date, bool recurrent);

private:
    std::unique_ptr<Ui::AddExceptionalDayDialog> ui;
    QDate startDate_;
    int numDays_{1};
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: ADDEXCEPTIONALDAYDIALOG_H_4KBYLJOJ */

