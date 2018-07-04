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
#ifndef PLOT_H
#define PLOT_H

#include <QEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QString>
#include <QWidget>

namespace sprint_timer::ui::qt_gui {

struct GraphPoint;

using GraphData = std::vector<GraphPoint>;

class Graph;

struct AxisRange {
    // Set minimum and maximum values on axis.
    // If start > end, then start is asssigned to end, and end to start.
    void setRange(double start, double end);

    // Return axis span, that is distance between minimum and maximum value.
    double span() const;

private:
    double start;
    double end;
};

class Plot : public QWidget {
    Q_OBJECT

private:
    struct PointBox {
        QPainterPath path;
        QPointF position;
        QString toolTip;
        QString label;
    };

public:
    using PointBoxContainer = std::vector<PointBox>;

    explicit Plot(QWidget* parent);

    // Add Graph to plot. Multiple graphs can be added.
    void addGraph(Graph graph);

    // Set data points to graph with given number.
    void setGraphData(size_t graphNum, GraphData& data);

    // Clear data points from all graphs in plot.
    // Empty graphs remain attached to Plot.
    void reset();

    // Repaint plot. This method is ment to be called when graph data is
    // changed.
    void replot();

    // Set visible axis range.
    void setRangeX(double start, double end);

    // Set visible axis range.
    void setRangeY(double start, double end);

    // Reserve space for n graphs.
    void setNumExpectedGraphs(size_t n);

protected:
    // Override to paint graphs.
    void paintEvent(QPaintEvent*) override;

    // Override to show tooltip with point data when hovering over the point.
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    std::vector<Graph> graphs;
    std::vector<PointBoxContainer> pointBoxes;
    AxisRange rangeX;
    AxisRange rangeY;
    QRectF availableRect;
    double pointBoxSize;

    // Compute data required to draw points relative to plot size.
    void constructPointBoxes();

    // Paint graph with given number.
    void paintGraph(size_t graphNum, QPainter& painter) const;

    // Paint graph points and labels.
    void paintPoints(const PointBoxContainer& boxes, QPainter& painter) const;

    // Show tooltip with point data when hovering over a point on a plot.
    const QString getPosTooltip(const QPoint& pos) const;
};

struct GraphPoint {
    double x;
    double y;
    QString label;
};

class Graph {
public:
    using const_iterator = GraphData::const_iterator;

    // Set data for this Graph.
    void setData(GraphData& data);

    // Set pen for drawing graph lines.
    void setPen(QPen& pen);

    // Return pen that is currently used for drawing graph lines.
    const QPen pen() const;

    // Overload for subscript operator to get access to graph points data.
    const GraphPoint& operator[](size_t idx) const;

    // Clear graph points.
    void clearData();

    // Draw data points and labels on graph if showPoints is true.
    // Draw graph only if showPoints is false.
    void setShowPoints(bool showPoints);

    // Return true if data points are supposed to be drawn and false otherwise.
    bool showPoints() const;

    // Return iterator to the beginning (first data point)
    const_iterator cbegin() const;

    // Return iterator to the end.
    const_iterator cend() const;

    // Return number of points in graph.
    size_t size() const;

private:
    QPen mPen;
    GraphData points;
    bool mShowPoints = false;
};

} // namespace sprint_timer::ui::qt_gui


#endif // PLOT_H
