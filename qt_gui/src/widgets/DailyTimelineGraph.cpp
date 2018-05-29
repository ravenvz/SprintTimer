/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

#include "widgets/DailyTimelineGraph.h"
#include "ui_daily_timeline_graph.h"
#include "widgets/Plot.h"
#include <QDate>
#include <QtWidgets/QGridLayout>


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
    constexpr double penWidthF = 2.2;

    Graph averageGraph;
    Graph goalGraph;
    Graph normalGraph;

    QPen averagePen;
    averagePen.setColor(Qt::blue);
    averagePen.setStyle(Qt::DotLine);
    averagePen.setWidthF(penWidthF);
    averageGraph.setPen(averagePen);

    QPen normalPen;
    normalPen.setColor(QColor::fromRgb(246, 61, 13, 255));
    normalPen.setWidthF(penWidthF);
    normalGraph.setPen(normalPen);
    normalGraph.setShowPoints(true);

    QPen goalPen;
    goalPen.setColor(Qt::green);
    goalPen.setStyle(Qt::DashLine);
    goalPen.setWidthF(penWidthF);
    goalGraph.setPen(goalPen);

    ui->dailyTimeline->setNumExpectedGraphs(3);
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

    if (dailyDistribution.empty()) {
        ui->dailyTimeline->reset();
        return;
    }
    else {
        auto sprintsByDay = dailyDistribution.getDistributionVector();
        GraphData averageData{
            GraphPoint{0, averagePerWorkday, ""},
            GraphPoint{static_cast<double>(dailyDistribution.getNumBins()),
                       averagePerWorkday,
                       ""}};
        GraphData goalData{
            GraphPoint{0, static_cast<double>(dailyGoal), ""},
            GraphPoint{static_cast<double>(dailyDistribution.getNumBins()),
                       static_cast<double>(dailyGoal),
                       ""}};
        GraphData normalData;
        for (size_t i = 0; i < sprintsByDay.size(); ++i) {
            normalData.push_back(
                GraphPoint{double(i),
                           sprintsByDay[i],
                           QString("%1").arg(
                               startDate.addDays(static_cast<long>(i)).day())});
        }

        ui->dailyTimeline->setRangeX(0, dailyDistribution.getNumBins() + 1);
        ui->dailyTimeline->setRangeY(0, dailyDistribution.getMax() + 1);
        ui->dailyTimeline->setGraphData(0, averageData);
        ui->dailyTimeline->setGraphData(1, goalData);
        ui->dailyTimeline->setGraphData(2, normalData);
    }

    ui->dailyTimeline->replot();
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
