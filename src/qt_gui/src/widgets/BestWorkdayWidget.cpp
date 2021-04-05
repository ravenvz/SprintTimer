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
#include "qt_gui/widgets/BestWorkdayWidget.h"
#include "qt_gui/widgets/BarChart.h"
#include <QDate>
#include <QLabel>
#include <QLocale>
#include <QSizePolicy>
#include <QVBoxLayout>

namespace sprint_timer::ui::qt_gui {

BestWorkdayWidget::BestWorkdayWidget(QWidget* parent_)
    : QWidget{parent_}
    , labelBestWorkdayName{std::make_unique<QLabel>().release()}
    , labelBestWorkdayMsg{std::make_unique<QLabel>().release()}
    , workdayBarChart{std::make_unique<BarChart>(nullptr).release()}
{
    auto layout_ = std::make_unique<QVBoxLayout>();

    auto labelTitle_ = std::make_unique<QLabel>("Best Workday");

    labelTitle_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelBestWorkdayName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelBestWorkdayMsg->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QFont labelFont{labelBestWorkdayName->font()};
    labelFont.setBold(true);
    labelBestWorkdayName->setFont(labelFont);

    layout_->addWidget(labelTitle_.release());
    layout_->addWidget(labelBestWorkdayName);
    layout_->addWidget(labelBestWorkdayMsg);
    layout_->addWidget(workdayBarChart);

    setLayout(layout_.release());
}

void BestWorkdayWidget::displayLegend(const LegendData& data)
{
    if (data.dayNum == -1) {
        labelBestWorkdayName->setText(QString::fromStdString(data.percentage));
        return;
    }
    const QLocale defaultLocale;
    labelBestWorkdayName->setText(
        defaultLocale.dayName(data.dayNum, QLocale::FormatType::LongFormat));
    labelBestWorkdayMsg->setText(
        QString("%1% more than average")
            .arg(QString::fromStdString(data.percentage)));
}

void BestWorkdayWidget::displayBars(const BarD& data)
{
    const QLocale defaultLocale;
    std::vector<QString> labels_;
    labels_.reserve(7);
    std::transform(cbegin(data.dayOrder),
                   cend(data.dayOrder),
                   std::back_inserter(labels_),
                   [&defaultLocale](const auto& elem) {
                       return defaultLocale.dayName(
                           elem, QLocale::FormatType::ShortFormat);
                   });
    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(QString::fromStdString(data.borderColor));
    workdayBarChart->setPen(pen);
    QBrush brush = QBrush(QColor(QString::fromStdString(data.barColor)));
    workdayBarChart->setBrush(brush);
    const BarData barData = BarData(data.barValues, labels_);
    workdayBarChart->setData(barData);
}

} // namespace sprint_timer::ui::qt_gui
