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
#include "qt_gui/widgets/DateRangePicker.h"
#include "ui_date_range_picker.h"
#include <QtCore/qdatetime.h>
#include <QtCore/qstringlistmodel.h>
#include <algorithm>
#include <iterator>
#include <memory>

#include "qt_gui/utils/DateTimeConverter.h"

#include <QDebug>

namespace {

dw::DateRange currentMonth();

QStringList monthNames();

} // namespace

namespace sprint_timer::ui::qt_gui {

DateRangePicker::DateRangePicker(
    std::unique_ptr<DateRangePickDialog> dateRangePickDialog_,
    QAbstractItemModel& yearsModel_,
    QWidget* parent_)
    : QWidget{parent_}
    , ui{std::make_unique<Ui::DateRangePicker>()}
    , dateRangePickDialog{std::move(dateRangePickDialog_)}
    , selectedDateRange{currentMonth()}
    , monthsModel{monthNames()}
{
    ui->setupUi(this);
    ui->cbxMonth->setModel(&monthsModel);
    ui->cbxYear->setModel(&yearsModel_);

    updateSelectionHintLabel();

    connect(ui->btnPickPeriod,
            &QPushButton::clicked,
            this,
            &DateRangePicker::openDatePickDialog);
    connect(ui->cbxYear,
            QOverload<int>::of(&QComboBox::activated),
            this,
            QOverload<>::of(&DateRangePicker::onRangeChanged));
    connect(ui->cbxMonth,
            QOverload<int>::of(&QComboBox::activated),
            this,
            QOverload<>::of(&DateRangePicker::onRangeChanged));
    connect(this,
            &DateRangePicker::selectedDateRangeChanged,
            this,
            &DateRangePicker::updateSelectionHintLabel);
    connect(&yearsModel_,
            &QAbstractItemModel::modelReset,
            this,
            &DateRangePicker::preselectCurrentYearMonth);
    connect(dateRangePickDialog.get(), &QDialog::accepted, [this]() {
        onRangeChanged(dateRangePickDialog->selectedRange());
    });
}

DateRangePicker::~DateRangePicker() = default;

void DateRangePicker::openDatePickDialog()
{
    dateRangePickDialog->setSelectionRange(selectedDateRange);
    dateRangePickDialog->show();
}

void DateRangePicker::updateSelectionHintLabel()
{
    // using QDate instead of dw::date here when converting to string
    // provides date localization
    ui->labelSelectionHint->setText(
        QString{"%1 - %2"}
            .arg(utils::toQDate(selectedDateRange.start()).toString())
            .arg(utils::toQDate(selectedDateRange.finish()).toString()));
}

void DateRangePicker::onRangeChanged()
{
    using namespace dw;
    const dw::Date start{
        Year{ui->cbxYear->currentText().toInt()},
        Month{static_cast<unsigned>(ui->cbxMonth->currentIndex() + 1)},
        Day{1}};
    const dw::Date finish = dw::last_day_of_month(start);
    selectedDateRange = DateRange{start, finish};

    emit selectedDateRangeChanged(selectedDateRange);
}

void DateRangePicker::onRangeChanged(const dw::DateRange& dateRange)
{
    selectedDateRange = dateRange;
    emit selectedDateRangeChanged(selectedDateRange);
}

dw::DateRange DateRangePicker::selectionRange() const
{
    return selectedDateRange;
}

void DateRangePicker::preselectCurrentYearMonth()
{
    const int firstYear{
        ui->cbxYear->itemData(0, Qt::DisplayRole).toString().toInt()};
    const int currentYear{dw::current_date_local().year()};
    const int offset{currentYear - firstYear};
    ui->cbxYear->setCurrentIndex(offset);

    ui->cbxMonth->setCurrentIndex(QDate::currentDate().month() - 1);
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
