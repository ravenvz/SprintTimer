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
#include "qt_gui/widgets/Plot.h"
#include <QHelpEvent>
#include <QToolTip>
#include <algorithm>

namespace {

using namespace sprint_timer::ui::qt_gui;

constexpr static int labelSkipInd{28};
constexpr static int toolTipOffset{50};
// RelSizes' are relative to Widget's rect() height
constexpr static double labelOffsetRelSize{0.15};
constexpr static double marginRelSize{0.07};
constexpr static double pointBoxRelSize{0.025};
const QBrush pointBoxBrush{Qt::white};

QRectF computeAvailableRectange(const QRectF& totalSizeRect);

struct point_to_point_box {
    point_to_point_box(const DrawingParams& drawingParams)
        : drawingParams{drawingParams}
    {
    }

    Graph::PointBox operator()(const Graph::Point& p)
    {
        QPainterPath path;
        // TODO figure out why MSVC doesn't like this structured binging
#ifdef WIN32
        auto labelOffset = drawingParams.labelOffset;
        const auto scaleX = drawingParams.scaleX;
        const auto scaleY = drawingParams.scaleY;
        const auto referencePoint = drawingParams.referencePoint;
        const auto pointSize = drawingParams.pointSize;
#else
        const auto& [labelOffset, scaleX, scaleY, referencePoint, pointSize] =
            drawingParams;
#endif
        const QPointF position{referencePoint.x() + p.x * scaleX,
                               referencePoint.y() - p.y * scaleY - labelOffset};
        path.addEllipse(QPointF{0, 0}, pointSize, pointSize);
        return Graph::PointBox{path, position, QString("%1").arg(p.y)};
    }

private:
    const DrawingParams& drawingParams;
};

} // namespace

namespace sprint_timer::ui::qt_gui {

void AxisRange::setRange(double start, double end)
{
    this->start = start;
    this->end = end;
}

void AxisRange::autoExpand(double start, double end)
{
    this->start = std::min(this->start, start);
    this->end = std::max(this->end, end);
}

double AxisRange::span() const { return end - start; }

Plot::Plot(QWidget* parent)
    : QWidget{parent}
{
    setMouseTracking(true);
}

DrawingParams::DrawingParams(const QRectF& availableRect,
                             const AxisRange& rangeX,
                             const AxisRange& rangeY)
    : labelOffset{labelOffsetRelSize * availableRect.height()}
    , scaleX{rangeX.span() > 1 ? availableRect.width() / (rangeX.span() - 1)
                               : 1}
    , scaleY{rangeY.span() > 1
                 ? (availableRect.height() - labelOffset) / rangeY.span()
                 : 1}
    , referencePoint{availableRect.bottomLeft()}
    , pointSize{pointBoxRelSize * availableRect.height()}
{
}

void Plot::addGraph(Graph graph) { graphs.push_back(std::move(graph)); }

void Plot::changeGraphData(size_t graphNum, Graph::Data&& data)
{
    if (graphNum < graphs.size())
        graphs[graphNum].setData(std::move(data));
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

    availableRect = computeAvailableRectange(this->rect());

    const DrawingParams drawingParams{availableRect, rangeX, rangeY};

    for (auto& graph : graphs)
        graph.draw(painter, drawingParams);
}

void Plot::mouseMoveEvent(QMouseEvent* event)
{
    for (const auto& graph : graphs)
        graph.handleMouseMoveEvent(event);
}

void Graph::setData(Data&& data)
{
    points = data;
    pointBoxes.resize(points.size());
    std::sort(points.begin(),
              points.end(),
              [](const auto& point1, const auto& point2) {
                  return point1.x < point2.x;
              });
}

void Graph::setVisualOptions(VisualOptions options_)
{
    options = std::move(options_);
}

void Graph::draw(QPainter& painter, const DrawingParams& drawingParams)
{
    painter.save();

    populatePointBoxes(drawingParams);
    drawPolyline(painter);
    drawPoints(painter);
    drawAxisLabels(painter, drawingParams.referencePoint.y());

    painter.restore();
}

void Graph::populatePointBoxes(const DrawingParams& drawingParams)
{
    std::transform(points.cbegin(),
                   points.cend(),
                   pointBoxes.begin(),
                   ::point_to_point_box(drawingParams));
}

void Graph::drawPolyline(QPainter& painter) const
{
    QPolygonF centerPoints;
    std::transform(pointBoxes.cbegin(),
                   pointBoxes.cend(),
                   std::back_inserter(centerPoints),
                   [](const auto& box) { return box.position; });
    painter.setPen(options.linePen);
    painter.drawPolyline(centerPoints);
}

void Graph::drawPoints(QPainter& painter)
{
    if (!options.showPoints)
        return;
    painter.setBrush(options.pointBrush);
    for (const auto& point : pointBoxes) {
        painter.translate(point.position);
        painter.drawPath(point.path);
        painter.translate(-point.position);
    }
}

void Graph::drawAxisLabels(QPainter& painter, double labelPosY)
{
    if (!options.showPoints)
        return;

    // For large spans not every label is shown to prevent overlapping.
    const size_t skipLabels{std::max(size_t{1}, points.size() / labelSkipInd)};

    for (size_t i = 0; i < pointBoxes.size(); i += skipLabels) {
        const auto& box = pointBoxes[i];
        painter.drawText(QPointF{box.position.x(), labelPosY}, points[i].label);
    }
}

void Graph::handleMouseMoveEvent(QMouseEvent* event) const
{
    if (!options.showPoints)
        return;

    // According to Qt documentation one should call event->ignore
    // if QToolTip::hideText called or QToolTip::showText is called
    // with empty string
    for (const auto& box : pointBoxes) {
        if (!box.path.contains(event->pos() - box.position))
            continue;
        const QPoint toolTipPos{event->globalPos().x(),
                                event->globalPos().y() - toolTipOffset};
        if (box.toolTip.isEmpty())
            event->ignore();
        QToolTip::showText(toolTipPos, box.toolTip);
        return;
    }
    QToolTip::hideText();
    event->ignore();
}

} // namespace sprint_timer::ui::qt_gui

namespace {

QRectF computeAvailableRectange(const QRectF& totalSizeRect)
{
    const double margin{marginRelSize * totalSizeRect.height()};
    return totalSizeRect.adjusted(margin, margin, -margin, -margin);
}

} // namespace
