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
#ifndef PLOT_H
#define PLOT_H

#include <QEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QString>
#include <QWidget>

namespace sprint_timer::ui::qt_gui {

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


struct DrawingParams {
    DrawingParams(const QRectF& availableRect,
                  const AxisRange& rangeX,
                  const AxisRange& rangeY);

    const double labelOffset;
    const double scaleX;
    const double scaleY;
    const QPointF referencePoint;
    const double pointSize;
};


class Graph {
    friend class point_to_point_box;

public:
    struct Point {
        double x;
        double y;
        QString label;
    };

    struct PointBox {
        QPainterPath path;
        QPointF position;
        QString toolTip;
    };

    struct VisualOptions {
        QPen linePen;
        QBrush pointBrush;
        QPen legendPen;
        bool showPoints{false};
    };

    using Data = std::vector<Point>;

    // Set data for this Graph.
    void setData(Graph::Data&& data);

    void setVisualOptions(VisualOptions options);

    void draw(QPainter& painter, const DrawingParams& drawingParams);

    void handleMouseMoveEvent(QMouseEvent* event) const;

private:
    VisualOptions options;
    Data points;
    std::vector<PointBox> pointBoxes;

    void populatePointBoxes(const DrawingParams& drawingParams);

    void drawPolyline(QPainter& painter) const;

    void drawPoints(QPainter& painter);

    void drawAxisLabels(QPainter& painter, double labelYPos);
};

class Plot : public QWidget {
    Q_OBJECT

public:
    explicit Plot(QWidget* parent);

    // Add Graph to plot. Multiple graphs can be added.
    void addGraph(Graph graph);

    // Set data points to graph with given number.
    void changeGraphData(size_t graphNum, Graph::Data&& data);

    // Set visible axis range.
    void setRangeX(double start, double end);

    // Set visible axis range.
    void setRangeY(double start, double end);

private:
    std::vector<Graph> graphs;
    AxisRange rangeX;
    AxisRange rangeY;
    QRectF availableRect;

    void paintEvent(QPaintEvent*) override;

    void mouseMoveEvent(QMouseEvent* event) override;
};

} // namespace sprint_timer::ui::qt_gui


#endif // PLOT_H
