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
#ifndef PICKPERIODWIDGET_H
#define PICKPERIODWIDGET_H

#include "qt_gui/presentation/DateRangeSelectorContract.h"
#include <QStringListModel>
#include <QWidget>
#include <core/IConfig.h>
#include <memory>

namespace Ui {
class DateRangePicker;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class DateRangeSelector : public QWidget,
                          public contracts::DateRangeSelectorContract::View {
public:
    explicit DateRangeSelector(QWidget* parent_ = nullptr);

    ~DateRangeSelector() override;

    void updateOperationalRange(const std::vector<int>& years) override;

    void setFirstDayOfWeek(dw::Weekday weekday) override;

private:
    std::unique_ptr<Ui::DateRangePicker> ui;
    std::unique_ptr<QAbstractItemModel> yearsModel;
    QStringListModel monthsModel;
    dw::DateRange selectedDateRange;
    dw::Weekday firstDayOfWeek{dw::Weekday::Sunday};

    void onYearOrMonthChanged();

    void onRangeChanged(const dw::DateRange& dateRange);

    void updateSelectionHintLabel();

    void openDatePickDialog();

    void preselectCurrentYearMonth();
};

} // namespace sprint_timer::ui::qt_gui

#endif // PICKPERIODWIDGET_H
