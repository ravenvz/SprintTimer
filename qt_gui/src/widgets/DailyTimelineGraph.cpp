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

namespace {

Qt::PenStyle
toPenStyle(sprint_timer::ui::contracts::DailyStatisticGraphContract::LineStyle
               lineStyle);

double computeTopY(
    const sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphData&
        data);

sprint_timer::ui::qt_gui::Graph::Data transformData(
    const sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphData&
        graphData);

sprint_timer::ui::qt_gui::Graph::VisualOptions
transformOptions(const sprint_timer::ui::contracts::
                     DailyStatisticGraphContract::GraphOptions& graphOptions);

} // namespace

namespace sprint_timer::ui::qt_gui {

namespace plot_params {

constexpr double penWidthF{2.2};
const QBrush pointBrush{Qt::white};
const QPen normalGraphPen{
    QColor::fromRgb(246, 61, 13, 255), penWidthF, Qt::SolidLine};
const QPen averageGraphPen{Qt::blue, penWidthF, Qt::DotLine};
const QPen goalGraphPen{Qt::green, penWidthF, Qt::DashLine};

} // namespace plot_params

DailyTimelineGraph::DailyTimelineGraph(
    BasePresenter<contracts::DailyStatisticGraphContract::View>& presenter_,
    QWidget* parent_)
    : QFrame{parent_}
    , ui{std::make_unique<Ui::DailyTimelineGraph>()}
    , presenter{presenter_}
{
    ui->setupUi(this);
    presenter_.attachView(*this);
}

DailyTimelineGraph::~DailyTimelineGraph() { presenter.detachView(*this); }

void DailyTimelineGraph::updateLegend(
    const contracts::DailyStatisticGraphContract::LegendData& data)
{
    ui->labelTotalSprints->setText(QString::fromStdString(data.total));
    ui->labelDailyAverage->setText(QString::fromStdString(data.average));
}

void DailyTimelineGraph::drawGraph(
    const contracts::DailyStatisticGraphContract::GraphData& graphData)
{

    Graph graph;
    auto data = transformData(graphData);
    const auto options = transformOptions(graphData.options);
    graph.setData(std::move(data));
    graph.setVisualOptions(options);

    ui->dailyTimeline->addGraph(graph);

    ui->dailyTimeline->setRangeX(0, data.size());
    ui->dailyTimeline->setRangeY(0, computeTopY(graphData));

    ui->dailyTimeline->repaint();
}

void DailyTimelineGraph::clearGraphs() { ui->dailyTimeline->clear(); }

} // namespace sprint_timer::ui::qt_gui

namespace {

Qt::PenStyle
toPenStyle(sprint_timer::ui::contracts::DailyStatisticGraphContract::LineStyle
               lineStyle)
{
    using sprint_timer::ui::contracts::DailyStatisticGraphContract::LineStyle;
    switch (lineStyle) {
    case LineStyle::Dash:
        return Qt::PenStyle::DashLine;
    case LineStyle::Dot:
        return Qt::PenStyle::DotLine;
    case LineStyle::Solid:
        return Qt::PenStyle::SolidLine;
    default:
        return Qt::PenStyle::NoPen;
    }
}

double computeTopY(
    const sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphData&
        graphData)
{
    const auto& values = graphData.values;
    if (values.empty())
        return 0;
    return std::max_element(values.cbegin(),
                            values.cend(),
                            [](const auto& lhs, const auto& rhs) {
                                return lhs.yValue.value < rhs.yValue.value;
                            })
        ->yValue.value;
}

sprint_timer::ui::qt_gui::Graph::Data transformData(
    const sprint_timer::ui::contracts::DailyStatisticGraphContract::GraphData&
        graphData)
{
    using sprint_timer::ui::qt_gui::Graph;
    const auto& values = graphData.values;
    Graph::Data data;
    data.reserve(values.size());

    auto entryToData = [](const auto& entry) {
        return Graph::Point{
            entry.xValue.value,
            entry.yValue.value,
            QString{"%1"}.arg(QString::fromStdString(entry.label))};
    };

    std::transform(
        values.cbegin(), values.cend(), std::back_inserter(data), entryToData);
    return data;
}

sprint_timer::ui::qt_gui::Graph::VisualOptions
transformOptions(const sprint_timer::ui::contracts::
                     DailyStatisticGraphContract::GraphOptions& graphOptions)
{
    using sprint_timer::ui::qt_gui::Graph;
    const Graph::VisualOptions options{
        QPen{QBrush{QColor{QString::fromStdString(graphOptions.penColor)}},
             graphOptions.penWidth,
             toPenStyle(graphOptions.style)},
        QBrush{QColor{QString::fromStdString(graphOptions.pointColor)}},
        QPen{QString::fromStdString(graphOptions.penColor)},
        graphOptions.showPoints};
    return options;
}

} // namespace
