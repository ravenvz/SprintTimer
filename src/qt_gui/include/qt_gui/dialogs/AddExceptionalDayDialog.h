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
#include <QDate>
#include <QDialog>
#include "date_wrapper/date_wrapper.h"
#include <memory>

#ifndef ADDEXCEPTIONALDAYDIALOG_H_4KBYLJOJ
#define ADDEXCEPTIONALDAYDIALOG_H_4KBYLJOJ

namespace sprint_timer::ui::qt_gui {

class AddExceptionalDayDialog : public QDialog {
public:
    struct OutputData {
        dw::Date startDate{dw::current_date_local()};
        int32_t numDays{1};
        int32_t sprintsPerDay{0};
    };

    AddExceptionalDayDialog(dw::Weekday firstDayOfWeek,
                            dw::Date preselectedDate,
                            OutputData& data,
                            QDialog* parent = nullptr);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: ADDEXCEPTIONALDAYDIALOG_H_4KBYLJOJ */

