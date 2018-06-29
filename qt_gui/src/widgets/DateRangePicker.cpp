/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "DateRangePicker.h"
#include "ui_date_range_picker.h"
#include <QtCore/qdatetime.h>
#include <QtCore/qstringlistmodel.h>
#include <algorithm>
#include <iterator>
#include <memory>


#include <QDebug>

namespace sprint_timer::ui::qt_gui {

DateRangePicker::DateRangePicker(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::DateRangePicker)
    , selectedInterval(DateInterval{
          QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1),
          QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1)
              .addMonths(1)
              .addDays(-1)})
{
    ui->setupUi(this);
    QStringList months;
    for (int monthNumber = 1; monthNumber <= 12; ++monthNumber) {
        months.append(QDate::longMonthName(monthNumber));
    }
    monthsModel = new QStringListModel(months, this);
    ui->cbxMonth->setModel(monthsModel);
    QString currentYear = QDate::currentDate().toString("yyyy");
    yearsModel = new QStringListModel({currentYear}, this);
    ui->cbxYear->setModel(yearsModel);

    connect(ui->btnPickPeriod,
            &QPushButton::clicked,
            this,
            &DateRangePicker::openDatePickDialog);
    connect(ui->cbxYear,
            qOverload<int>(&QComboBox::activated),
            this,
            qOverload<>(&DateRangePicker::updateInterval));
    connect(ui->cbxMonth,
            qOverload<int>(&QComboBox::activated),
            this,
            qOverload<>(&DateRangePicker::updateInterval));
    connect(this,
            &DateRangePicker::timeSpanChanged,
            this,
            &DateRangePicker::updateSelectionHintLabel);
}

DateRangePicker::~DateRangePicker() { delete ui; }

void DateRangePicker::openDatePickDialog()
{
    DateRangePickDialog dialog{selectedInterval};
    if (dialog.exec()) {
        updateInterval(dialog.getNewInterval());
    }
}

void DateRangePicker::updateSelectionHintLabel()
{
    ui->labelSelectionHint->setText(selectedInterval.toString());
}

void DateRangePicker::setInterval(DateInterval&& dateInterval)
{
    selectedInterval = std::move(dateInterval);
    emit timeSpanChanged(selectedInterval);
}

void DateRangePicker::updateInterval()
{
    QDate startDate{ui->cbxYear->currentText().toInt(),
                    ui->cbxMonth->currentIndex() + 1,
                    1};
    QDate endDate = startDate.addDays(startDate.daysInMonth() - 1);
    selectedInterval = DateInterval{startDate, endDate};
    emit timeSpanChanged(selectedInterval);
}

void DateRangePicker::updateInterval(DateInterval timeSpan)
{
    selectedInterval = timeSpan;
    emit timeSpanChanged(selectedInterval);
}

DateInterval DateRangePicker::getInterval() const { return selectedInterval; }

void DateRangePicker::setYears(const std::vector<std::string>& years)
{
    QStringList yearRange;
    std::transform(
        years.cbegin(),
        years.cend(),
        std::back_inserter(yearRange),
        [](const auto& elem) { return QString::fromStdString(elem); });

    if (!years.empty())
        yearsModel->setStringList(yearRange);

    ui->cbxYear->setCurrentIndex(static_cast<int>(std::distance(
        yearRange.begin(),
        std::find(yearRange.begin(),
                  yearRange.end(),
                  QString("%1").arg(QDate::currentDate().year())))));
    ui->cbxMonth->setCurrentIndex(QDate::currentDate().month() - 1);
    updateInterval();
}

} // namespace sprint_timer::ui::qt_gui
