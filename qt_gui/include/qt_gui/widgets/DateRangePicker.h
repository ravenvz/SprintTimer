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
#ifndef PICKPERIODWIDGET_H
#define PICKPERIODWIDGET_H

#include "qt_gui/dialogs/DateRangePickDialog.h"
#include "qt_gui/models/OperationRangeModel.h"
#include <QStringListModel>
#include <core/IConfig.h>
#include <memory>

namespace Ui {
class DateRangePicker;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class DateRangePicker : public QWidget {

    Q_OBJECT

public:
    DateRangePicker(std::unique_ptr<DateRangePickDialog> dateRangePickDialog,
                    const OperationRangeModel& operationRangeModel,
                    QWidget* parent = nullptr);

    ~DateRangePicker() override;

    dw::DateRange selectionRange() const;

signals:
    void selectedDateRangeChanged(const dw::DateRange&);

private:
    std::unique_ptr<Ui::DateRangePicker> ui;
    std::unique_ptr<DateRangePickDialog> dateRangePickDialog;
    const OperationRangeModel& operationRangeModel;
    dw::DateRange selectedDateRange;
    QStringListModel monthsModel;
    QStringListModel yearsModel;

    void openDatePickDialog();

    void onRangeChanged();

    void onRangeChanged(const dw::DateRange& dateRange);

    void updateSelectionHintLabel();

    void setOperationalRange(const dw::DateRange& operationalRange);

    void selectCurrentYearMonth(const QStringList& yearRange);
};

} // namespace sprint_timer::ui::qt_gui


#endif // PICKPERIODWIDGET_H
