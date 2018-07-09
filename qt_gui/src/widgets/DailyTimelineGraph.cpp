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
    , ui{new Ui::DailyTimelineGraph}
{
    ui->setupUi(this);
    setupGraphs();
}

DailyTimelineGraph::~DailyTimelineGraph() { delete ui; }

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
                                 const QDate& startDate,
                                 int numWorkdays,
                                 int dailyGoal)
{
    const double averagePerWorkday
        = dailyDistribution.getTotal() / static_cast<double>(numWorkdays);

    const auto sprintsByDay = dailyDistribution.getDistributionVector();
    Graph::Data averageData{
        Graph::Point{0, averagePerWorkday, ""},
        Graph::Point{static_cast<double>(dailyDistribution.getNumBins()),
                     averagePerWorkday,
                     ""}};
    Graph::Data goalData{
        Graph::Point{0, static_cast<double>(dailyGoal), ""},
        Graph::Point{static_cast<double>(dailyDistribution.getNumBins()),
                     static_cast<double>(dailyGoal),
                     ""}};
    Graph::Data normalData;
    for (size_t i = 0; i < sprintsByDay.size(); ++i) {
        normalData.push_back(Graph::Point{
            double(i),
            sprintsByDay[i],
            QString("%1").arg(startDate.addDays(static_cast<long>(i)).day())});
    }

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
