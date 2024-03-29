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
#include "qt_gui/widgets/DateRangeSelector.h"
#include "qt_gui/dialogs/DateRangePickDialog.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "ui_date_range_picker.h"
#include "QtCore/qdatetime.h"
#include "QtCore/qstringlistmodel.h"
#include <algorithm>
#include <iterator>
#include <memory>

namespace {

dw::DateRange currentMonth();

QStringList monthNames();

} // namespace

namespace sprint_timer::ui::qt_gui {

DateRangeSelector::DateRangeSelector(QWidget* parent_)
    : QWidget{parent_}
    , ui{std::make_unique<Ui::DateRangePicker>()}
    , monthsModel{monthNames()}
    , selectedDateRange{currentMonth()}
{
    ui->setupUi(this);
    ui->cbxMonth->setModel(&monthsModel);

    connect(ui->btnPickPeriod,
            &QPushButton::clicked,
            this,
            &DateRangeSelector::openDatePickDialog);
    connect(ui->cbxYear,
            QOverload<int>::of(&QComboBox::activated),
            this,
            QOverload<>::of(&DateRangeSelector::onYearOrMonthChanged));
    connect(ui->cbxMonth,
            QOverload<int>::of(&QComboBox::activated),
            this,
            QOverload<>::of(&DateRangeSelector::onYearOrMonthChanged));
}

DateRangeSelector::~DateRangeSelector() { }

void DateRangeSelector::updateOperationalRange(const std::vector<int>& years)
{
    QStringList lst;
    std::transform(cbegin(years),
                   cend(years),
                   std::back_inserter(lst),
                   [](const auto& elem) { return QString("%1").arg(elem); });
    yearsModel = std::make_unique<QStringListModel>(lst);

    ui->cbxMonth->setModel(&monthsModel);
    ui->cbxYear->setModel(yearsModel.get());
    updateSelectionHintLabel();
    preselectCurrentYearMonth();
}

void DateRangeSelector::setFirstDayOfWeek(dw::Weekday weekday)
{
    firstDayOfWeek = weekday;
}

void DateRangeSelector::onYearOrMonthChanged()
{
    using namespace dw;
    if (auto p = presenter(); p) {
        const dw::Date start{
            Year{ui->cbxYear->currentText().toInt()},
            Month{static_cast<unsigned>(ui->cbxMonth->currentIndex() + 1)},
            Day{1}};
        const dw::Date finish = dw::last_day_of_month(start);
        selectedDateRange = DateRange{start, finish};
        p.value()->onSelectedRangeChanged(selectedDateRange);
        updateSelectionHintLabel();
    }
}

void DateRangeSelector::onRangeChanged(const dw::DateRange& dateRange)
{
    selectedDateRange = dateRange;
    if (auto p = presenter(); p) {
        p.value()->onSelectedRangeChanged(selectedDateRange);
        updateSelectionHintLabel();
    }
}

void DateRangeSelector::updateSelectionHintLabel()
{
    // using QDate instead of dw::date here when converting to string
    // provides date localization
    ui->labelSelectionHint->setText(
        QString{"%1 - %2"}
            .arg(utils::toQDate(selectedDateRange.start()).toString())
            .arg(utils::toQDate(selectedDateRange.finish()).toString()));
}

void DateRangeSelector::openDatePickDialog()
{
    DateRangePickDialog dialog{firstDayOfWeek, selectedDateRange};
    if (dialog.exec() == QDialog::Accepted) {
        onRangeChanged(dialog.selectedRange());
    }
}

void DateRangeSelector::preselectCurrentYearMonth()
{
    using namespace dw;
    const int firstYear{
        ui->cbxYear->itemData(0, Qt::DisplayRole).toString().toInt()};
    const Date currentDate{current_date_local()};
    const int currentYear{currentDate.year()};
    const int offset{currentYear - firstYear};
    ui->cbxYear->setCurrentIndex(offset);
    ui->cbxMonth->setCurrentIndex(QDate::currentDate().month() - 1);
    onRangeChanged(
        DateRange{Date{currentDate.year(), currentDate.month(), Day{1}},
                  last_day_of_month(currentDate)});
}

} // namespace sprint_timer::ui::qt_gui

namespace {

dw::DateRange currentMonth()
{
    const auto today = dw::current_date_local();
    const dw::Date firstDayOfMonth{today.year(), today.month(), dw::Day{1}};
    const dw::Date lastDayOfMonth{dw::last_day_of_month(today)};
    return dw::DateRange{firstDayOfMonth, lastDayOfMonth};
}

QStringList monthNames()
{
    QStringList months;
    const QLocale defaultLocale;
    for (int monthNumber = 1; monthNumber <= 12; ++monthNumber) {
        months.append(defaultLocale.monthName(monthNumber,
                                              QLocale::FormatType::LongFormat));
    }
    return months;
}

} // namespace
