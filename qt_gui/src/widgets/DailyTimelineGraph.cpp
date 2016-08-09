/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#include "widgets/DailyTimelineGraph.h"
#include "ui_daily_timeline_graph.h"
#include "widgets/plot.h"
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
    Graph averageGraph = Graph();
    Graph goalGraph = Graph();
    Graph normalGraph = Graph();

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

    ui->dailyTimeline->addGraph(averageGraph);
    ui->dailyTimeline->addGraph(goalGraph);
    ui->dailyTimeline->addGraph(normalGraph);
}

void DailyTimelineGraph::setData(const Distribution<double>& dailyDistribution,
                                 const QDate& startDate,
                                 int dailyGoal)
{


    if (dailyDistribution.empty()) {
        ui->dailyTimeline->reset();
    }
    else {
        double average = dailyDistribution.getAverage();
        auto pomosByDay = dailyDistribution.getDistributionVector();
        GraphData averageData{
            GraphPoint{0, average, ""},
            GraphPoint{static_cast<double>(dailyDistribution.getNumBins()),
                       average,
                       ""}};
        GraphData goalData{
            GraphPoint{0, static_cast<double>(dailyGoal), ""},
            GraphPoint{static_cast<double>(dailyDistribution.getNumBins()),
                       static_cast<double>(dailyGoal),
                       ""}};
        GraphData normalData;
        for (size_t i = 0; i < pomosByDay.size(); ++i) {
            normalData.push_back(
                GraphPoint{double(i),
                           pomosByDay[i],
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
    updateLegend(dailyDistribution);
}

void DailyTimelineGraph::updateLegend(
    const Distribution<double>& dailyDistribution)
{
    ui->labelTotalPomodoros->setText(
        QString("%1").arg(dailyDistribution.getTotal()));
    ui->labelDailyAverage->setText(
        QString("%1").arg(dailyDistribution.getAverage(), 2, 'f', 2, '0'));
}
