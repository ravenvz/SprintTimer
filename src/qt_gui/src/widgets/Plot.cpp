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
#include "qt_gui/widgets/Plot.h"
#include <QHelpEvent>
#include <QToolTip>
#include <algorithm>

namespace {

using namespace sprint_timer::ui::qt_gui;

constexpr int labelSkipInd{28};
constexpr int toolTipOffset{40};
// RelSizes' are relative to Widget's rect() height
constexpr double labelOffsetRelSize{0.15};
constexpr double marginRelSize{0.07};
constexpr double pointBoxRelSize{0.025};
constexpr double labelRectWidth{30.0};
constexpr double labelRectHeight{20.0};
const QBrush pointBoxBrush{Qt::white};

QRectF computeAvailableRectange(const QRectF& totalSizeRect);

QPointF translated(const Graph::Point& point,
                   const Graph::ResizingScaler& resizingScaler);

} // namespace

namespace sprint_timer::ui::qt_gui {

void AxisRange::setRange(double start_, double end_)
{
    start = start_;
    end = end_;
}

void AxisRange::autoExpand(double start_, double end_)
{
    start = std::min(start_, start);
    end = std::max(end_, end);
}

double AxisRange::span() const { return end - start; }

Plot::Plot(QWidget* parent)
    : QWidget{parent}
{
    setMouseTracking(true);
}

Graph::ResizingScaler::ResizingScaler(const QRectF& canvas,
                                      const AxisRange& rangeX,
                                      const AxisRange& rangeY)
    : labelOffset{labelOffsetRelSize * canvas.height()}
    , scaleX{rangeX.span() > 1 ? canvas.width() / (rangeX.span() - 1) : 1}
    , scaleY{rangeY.span() > 1 ? (canvas.height() - labelOffset) / rangeY.span()
                               : 1}
    , referencePoint{canvas.bottomLeft()}
{
    const auto pointSize{pointBoxRelSize * canvas.height()};
    pointPath.addEllipse(QPointF{0, 0}, pointSize, pointSize);
}

void Plot::addGraph(Graph graph) { graphs.push_back(std::move(graph)); }

void Plot::changeGraphData(size_t graphNum, Graph::Data&& data_)
{
    if (graphNum < graphs.size())
        graphs[graphNum].setData(std::move(data_));
}

void Plot::changeGraphVisualOptions(size_t graphNum,
                                    Graph::VisualOptions options)
{
    if (graphNum < graphs.size())
        graphs[graphNum].setVisualOptions(std::move(options));
}

void Plot::setRangeX(double start, double end)
{
    rangeX.autoExpand(start, end);
}

void Plot::setRangeY(double start, double end)
{
    rangeY.autoExpand(start, end);
}

void Plot::clear()
{
    graphs.clear();
    rangeX.setRange(0, 0);
    rangeY.setRange(0, 0);
    repaint();
}

void Plot::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (auto& graph : graphs)
        graph.draw(painter, this->rect(), rangeX, rangeY);
}

void Plot::mouseMoveEvent(QMouseEvent* event)
{
    for (const auto& graph : graphs)
        graph.handleMouseMoveEvent(event);
}

Graph::PointBox::PointBox(QPointF position, QString toolTip)
    : position{position}
    , toolTip{toolTip}
{
}

void Graph::setData(Data&& data)
{
    points = std::move(data);
    // For large spans not every label is shown to prevent overlapping.
    labelSkipStep = std::max(size_t{1}, points.size() / labelSkipInd);
}

void Graph::setVisualOptions(VisualOptions options_)
{
    options = std::move(options_);
}

void Graph::draw(QPainter& painter,
                 const QRectF& canvasRect,
                 const AxisRange& rangeX,
                 const AxisRange& rangeY)
{
    resizingScaler =
        ResizingScaler{computeAvailableRectange(canvasRect), rangeX, rangeY};

    painter.save();

    drawPolyline(painter);
    drawPoints(painter);
    drawAxisLabels(painter);

    painter.restore();
}

void Graph::drawPolyline(QPainter& painter) const
{
    painter.setPen(options.linePen);
    std::adjacent_find(cbegin(points),
                       cend(points),
                       [&](const auto& current, const auto& next) {
                           painter.drawLine(translated(current, resizingScaler),
                                            translated(next, resizingScaler));
                           return false;
                       });
}

void Graph::drawPoints(QPainter& painter) const
{
    if (!options.showPoints)
        return;
    painter.setBrush(options.pointBrush);
    for (const auto& point : points) {
        const auto position = translated(point, resizingScaler);
        painter.translate(translated(point, resizingScaler));
        painter.drawPath(resizingScaler.pointPath);
        painter.translate(-position);
    }
}

void Graph::drawAxisLabels(QPainter& painter) const
{
    if (!options.showPoints)
        return;

    QRectF labelRect{0, 0, labelRectWidth, labelRectHeight};

    for (size_t i = 0; i < points.size(); i += labelSkipStep) {
        const auto& point = points[i];
        const auto pos = translated(points[i], resizingScaler);
        labelRect.moveCenter(
            QPointF{pos.x(), resizingScaler.referencePoint.y()});
        painter.drawText(labelRect, Qt::AlignCenter, point.label);
    }
}

void Graph::handleMouseMoveEvent(QMouseEvent* event) const
{
    if (!options.showPoints) {
        return;
    }

    auto cursorInsidePointImage = [&](const auto& point) {
        return resizingScaler.pointPath.contains(
            event->pos() - translated(point, resizingScaler));
    };
    auto pointIt =
        std::find_if(cbegin(points), cend(points), cursorInsidePointImage);

    // According to Qt documentation one should call event->ignore
    // if QToolTip::hideText called or QToolTip::showText is called
    // with empty string
    if (pointIt == cend(points) || pointIt->label.isEmpty()) {
        QToolTip::hideText();
        event->ignore();
        return;
    }

    auto [x, y] = event->globalPosition().toPoint();
    const QPoint toolTipPos{QPoint{x, y - toolTipOffset}};
    QToolTip::showText(toolTipPos,
                       QString{"%1"}.arg(static_cast<int>(pointIt->y)));
}

} // namespace sprint_timer::ui::qt_gui

namespace {

QRectF computeAvailableRectange(const QRectF& totalSizeRect)
{
    const double margin{marginRelSize * totalSizeRect.height()};
    return totalSizeRect.adjusted(margin, margin, -margin, -margin);
}

QPointF translated(const Graph::Point& point,
                   const Graph::ResizingScaler& resizingScaler)
{
    const auto& [labelOffset, scaleX, scaleY, referencePoint, pointPath] =
        resizingScaler;
    return QPointF{referencePoint.x() + point.x * scaleX,
                   referencePoint.y() - point.y * scaleY - labelOffset};
}

} // namespace
