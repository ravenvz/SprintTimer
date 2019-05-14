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
#ifndef DATEPICKDIALOG_H
#define DATEPICKDIALOG_H

#include "qt_gui/utils/DateInterval.h"
#include <QDialog>
#include <core/IConfig.h>
#include <memory>

namespace Ui {
class DateRangePickDialog;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class DateRangePickDialog : public QDialog {
    Q_OBJECT

public:
    DateRangePickDialog(const IConfig& applicationSettings,
                        QWidget* parent = nullptr);

    ~DateRangePickDialog() override;

    dw::DateRange selectedRange();

    void setSelectionRange(const dw::DateRange& dateRange);

private:
    std::unique_ptr<Ui::DateRangePickDialog> ui;
    const IConfig& applicationSettings;

    void showEvent(QShowEvent*) override;
};

} // namespace sprint_timer::ui::qt_gui


#endif // DATEPICKDIALOG_H
