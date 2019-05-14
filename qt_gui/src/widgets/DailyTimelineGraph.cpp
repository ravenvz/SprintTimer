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
#include "qt_gui/widgets/DailyTimelineGraph.h"
#include "qt_gui/widgets/Plot.h"
#include "ui_daily_timeline_graph.h"
#include <QDate>
#include <QtWidgets/QGridLayout>

namespace sprint_timer::ui::qt_gui {

namespace plot_params {

    constexpr double penWidthF{2.2};
    const QBrush pointBrush{Qt::white};
    const QPen normalGraphPen{
        QColor::fromRgb(246, 61, 13, 255), penWidthF, Qt::SolidLine};
    const QPen averageGraphPen{Qt::blue, penWidthF, Qt::DotLine};
    const QPen goalGraphPen{Qt::green, penWidthF, Qt::DashLine};

} // namespace plot_params

DailyTimelineGraph::DailyTimelineGraph(QWidget* parent)
    : QFrame{parent}
    , ui{std::make_unique<Ui::DailyTimelineGraph>()}
{
    ui->setupUi(this);
    setupGraphs();
}

DailyTimelineGraph::~DailyTimelineGraph() = default;

void DailyTimelineGraph::setupGraphs()
{
    Graph averageGraph;
    Graph normalGraph;
    Graph goalGraph;

    Graph::VisualOptions averageGraphOptions;
    averageGraphOptions.linePen = plot_params::averageGraphPen;
    averageGraph.setVisualOptions(std::move(averageGraphOptions));

    Graph::VisualOptions normalGraphOptions;
    normalGraphOptions.linePen = plot_params::normalGraphPen;
    normalGraphOptions.showPoints = true;
    normalGraphOptions.pointBrush = plot_params::pointBrush;
    normalGraph.setVisualOptions(std::move(normalGraphOptions));

    Graph::VisualOptions goalGraphOptions;
    goalGraphOptions.linePen = plot_params::goalGraphPen;
    goalGraph.setVisualOptions(std::move(goalGraphOptions));

    ui->dailyTimeline->addGraph(std::move(averageGraph));
    ui->dailyTimeline->addGraph(std::move(goalGraph));
    ui->dailyTimeline->addGraph(std::move(normalGraph));
}

void DailyTimelineGraph::setData(const Distribution<double>& dailyDistribution,
                                 const dw::DateRange& dateRange,
                                 int numWorkdays,
                                 int goalForPeriod)
{
    const double averagePerWorkday = (numWorkdays == 0)
        ? 0.0
        : dailyDistribution.getTotal() / static_cast<double>(numWorkdays);

    const auto sprintsByDay = dailyDistribution.getDistributionVector();
    Graph::Data averageData{
        Graph::Point{0, averagePerWorkday, ""},
        Graph::Point{static_cast<double>(dailyDistribution.getNumBins()),
                     averagePerWorkday,
                     ""}};
    const double averagedGoal
        = (numWorkdays == 0) ? 0 : goalForPeriod / numWorkdays;
    Graph::Data goalData{
        Graph::Point{0, static_cast<double>(averagedGoal), ""},
        Graph::Point{static_cast<double>(dailyDistribution.getNumBins()),
                     static_cast<double>(averagedGoal),
                     ""}};
    Graph::Data normalData;
    normalData.reserve(sprintsByDay.size());
    int position{0};
    std::transform(
        sprintsByDay.cbegin(),
        sprintsByDay.cend(),
        std::back_inserter(normalData),
        [&](double value) {
            const dw::Date date{dateRange.start() + dw::Days{position}};
            const auto dayNumber = static_cast<unsigned>(date.day());
            return Graph::Point{
                double(position++), value, QString{"%1"}.arg(dayNumber)};
        });

    ui->dailyTimeline->setRangeX(0, dailyDistribution.getNumBins() + 1);
    ui->dailyTimeline->setRangeY(0, dailyDistribution.getMax() + 1);
    ui->dailyTimeline->changeGraphData(0, std::move(averageData));
    ui->dailyTimeline->changeGraphData(1, std::move(goalData));
    ui->dailyTimeline->changeGraphData(2, std::move(normalData));

    ui->dailyTimeline->repaint();
    updateLegend(dailyDistribution, averagePerWorkday);
}

void DailyTimelineGraph::updateLegend(
    const Distribution<double>& dailyDistribution, double averagePerWorkday)
{
    ui->labelTotalSprints->setText(
        QString("%1").arg(dailyDistribution.getTotal()));
    ui->labelDailyAverage->setText(
        QString("%1").arg(averagePerWorkday, 2, 'f', 2, '0'));
}

} // namespace sprint_timer::ui::qt_gui
