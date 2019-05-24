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

BestWorkdayWidget::BestWorkdayWidget(QWidget* parent)
    : QWidget(parent)
    , ui{std::make_unique<Ui::BestWorkdayWidget>()}
{
    ui->setupUi(this);
    for (int i = 0; i < 7; ++i) {
        labels.push_back(QDate::shortDayName(i + 1));
    }
    setupWeekdayBarChart();
}

BestWorkdayWidget::~BestWorkdayWidget() = default;

void BestWorkdayWidget::setupWeekdayBarChart()
{
    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(Qt::red);
    ui->workdayBarChart->setPen(pen);
    QBrush brush = QBrush(QColor("#73c245"));
    ui->workdayBarChart->setBrush(brush);
}

void BestWorkdayWidget::setData(const std::vector<entities::Sprint>& sprints,
                                const dw::DateRange& dateRange)
{
    const auto distribution = weekdayStatistics(sprints, dateRange);
    updateWeekdayBarChart(distribution);
    updateWeekdayBarChartLegend(distribution);
}

void BestWorkdayWidget::updateWeekdayBarChart(
    const Distribution<double>& weekdayDistribution)
{
    std::vector<double> values = weekdayDistribution.getDistributionVector();
    const BarData data = BarData(values, labels);
    ui->workdayBarChart->setData(data);
}

void BestWorkdayWidget::updateWeekdayBarChartLegend(
    const Distribution<double>& weekdayDistribution)
{
    if (weekdayDistribution.empty()) {
        ui->labelBestWorkdayName->setText("No data");
        ui->labelBestWorkdayMsg->setText("");
        return;
    }

    const double average = weekdayDistribution.getAverage();
    const int relativeComparisonInPercent
        = int((weekdayDistribution.getMax() - average) * 100 / average);
    ui->labelBestWorkdayName->setText(QDate::longDayName(
        static_cast<int>(weekdayDistribution.getMaxValueBin()) + 1));
    ui->labelBestWorkdayMsg->setText(
        QString("%1% more than average").arg(relativeComparisonInPercent));
}

} // namespace sprint_timer::ui::qt_gui
