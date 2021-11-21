/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include <QPainterPath>
#include <QString>
#include <QWidget>

namespace sprint_timer::ui::qt_gui {

struct AxisRange {
    void autoExpand(double start, double end);

    void setRange(double start, double end);

    // Return axis span, that is distance between minimum and maximum value.
    double span() const;

private:
    double start{0};
    double end{0};
};

class Graph {
public:
    struct Point {
        double x;
        double y;
        QString label;
    };

    struct PointBox {
        QPointF position;
        QString toolTip;

        PointBox() = default;

        PointBox(QPointF position, QString toolTip);
    };

    struct VisualOptions {
        QPen linePen;
        QBrush pointBrush;
        QPen legendPen;
        bool showPoints{false};
    };

    struct ResizingScaler {
        ResizingScaler() = default;

        ResizingScaler(const QRectF& availableRect,
                       const AxisRange& rangeX,
                       const AxisRange& rangeY);

        double labelOffset;
        double scaleX;
        double scaleY;
        QPointF referencePoint;
        QPainterPath pointPath;
    };

    using Data = std::vector<Point>;

    // Assumes that data is sorted by x coordinate in ascending order.
    void setData(Graph::Data&& data);

    void setVisualOptions(VisualOptions options);

    void draw(QPainter& painter,
              const QRectF& canvasRect,
              const AxisRange& rangeX,
              const AxisRange& rangeY);

    void handleMouseMoveEvent(QMouseEvent* event) const;

private:
    VisualOptions options;
    ResizingScaler resizingScaler;
    Data points;
    size_t labelSkipStep;

    void drawPolyline(QPainter& painter) const;

    void drawPoints(QPainter& painter) const;

    void drawAxisLabels(QPainter& painter) const;
};

class Plot : public QWidget {
public:
    explicit Plot(QWidget* parent);

    // Add Graph to plot. Multiple graphs can be added.
    void addGraph(Graph graph);

    void addGraph(Graph graph, Graph::VisualOptions options);

    // Set data points to graph with given number.
    void changeGraphData(size_t graphNum, Graph::Data&& data);

    void changeGraphVisualOptions(size_t graphNum,
                                  Graph::VisualOptions options);

    // Set visible axis range.
    void setRangeX(double start, double end);

    // Set visible axis range.
    void setRangeY(double start, double end);

    void clear();

private:
    std::vector<Graph> graphs;
    AxisRange rangeX;
    AxisRange rangeY;

    void paintEvent(QPaintEvent*) override;

    void mouseMoveEvent(QMouseEvent* event) override;
};

} // namespace sprint_timer::ui::qt_gui

#endif // PLOT_H
