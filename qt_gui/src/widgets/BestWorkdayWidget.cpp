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
#include "ui_best_workday_widget.h"
#include <QDate>

namespace sprint_timer::ui::qt_gui {

BestWorkdayWidget::BestWorkdayWidget(
    BasePresenter<contracts::BestWorkday::View>& presenter_, QWidget* parent_)
    : QWidget{parent_}
    , ui{std::make_unique<Ui::BestWorkdayWidget>()}
    , presenter{presenter_}
{
    ui->setupUi(this);
    presenter.attachView(*this);
}

BestWorkdayWidget::~BestWorkdayWidget() { presenter.detachView(*this); }

void BestWorkdayWidget::displayLegend(const LegendData& data)
{
    if (data.dayNum == -1) {
        ui->labelBestWorkdayName->setText(
            QString::fromStdString(data.percentage));
        return;
    }
    const QLocale defaultLocale;
    ui->labelBestWorkdayName->setText(
        defaultLocale.dayName(data.dayNum, QLocale::FormatType::LongFormat));
    ui->labelBestWorkdayMsg->setText(
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
    ui->workdayBarChart->setPen(pen);
    QBrush brush = QBrush(QColor(QString::fromStdString(data.barColor)));
    ui->workdayBarChart->setBrush(brush);
    const BarData barData = BarData(data.barValues, labels_);
    ui->workdayBarChart->setData(barData);
}

} // namespace sprint_timer::ui::qt_gui
