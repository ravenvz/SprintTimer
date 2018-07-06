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
#include "qt_gui/widgets/Plot.h"
#include <QHelpEvent>
#include <QToolTip>

namespace {

using namespace sprint_timer::ui::qt_gui;

constexpr static int labelSkipInd{28};
constexpr static int toolTipOffset{50};
// RelSizes' are relative to Widget's rect() height
constexpr static double labelOffsetRelSize{0.15};
constexpr static double marginRelSize{0.07};
constexpr static double pointBoxRelSize{0.025};
const QBrush pointBoxBrush{Qt::white};

struct PointBoxScale {
    PointBoxScale(const QRectF& availableRect,
                  const AxisRange& rangeX,
                  const AxisRange& rangeY);

    const double labelOffset;
    const double scaleX;
    const double scaleY;
    const QPointF referencePoint;
    const double pbSize;
};

QRectF computeAvailableRectange(const QRectF& totalSizeRect);

struct point_to_point_box {
    point_to_point_box(const PointBoxScale& scale)
        : scale{scale}
    {
    }

    Graph::PointBox operator()(const GraphPoint& p)
    {
        QPainterPath path;
        auto [labelOffset, scaleX, scaleY, referencePoint, pbSize] = scale;
        const QPointF position{p.x * scaleX + referencePoint.x(),
                               referencePoint.y() - p.y * scaleY - labelOffset};
        path.addEllipse(QPointF{0, 0}, pbSize, pbSize);
        return Graph::PointBox{path, position, QString("%1").arg(p.y), p.label};
    }

private:
    const PointBoxScale& scale;
};

} // namespace

namespace sprint_timer::ui::qt_gui {

void AxisRange::setRange(double start, double end)
{
    this->start = std::min(start, end);
    this->end = std::max(start, end);
}

double AxisRange::span() const { return end - start; }

Plot::Plot(QWidget* parent)
    : QWidget{parent}
{
    setMouseTracking(true);
}

void Plot::setNumExpectedGraphs(size_t n) { graphs.reserve(n); }

void Plot::addGraph(Graph graph) { graphs.push_back(std::move(graph)); }

void Plot::setGraphData(size_t graphNum, GraphData& data)
{
    if (graphNum < graphs.size())
        graphs[graphNum].setData(data);
}

void Plot::reset()
{
    for (auto& graph : graphs) {
        graph.clearData();
    }
}

void Plot::replot() { repaint(); }

void Plot::setRangeX(double start, double end) { rangeX.setRange(start, end); }

void Plot::setRangeY(double start, double end) { rangeY.setRange(start, end); }

void Plot::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    availableRect = computeAvailableRectange(this->rect());

    for (auto& graph : graphs)
        graph.draw(painter, availableRect, rangeX, rangeY);
}

void Plot::mouseMoveEvent(QMouseEvent* event)
{
    for (const auto& graph : graphs)
        graph.handleMouseMoveEvent(event);
}

void Graph::setData(GraphData& data)
{
    points = data;
    pointBoxes.resize(points.size());
    std::sort(points.begin(),
              points.end(),
              [](const auto& point1, const auto& point2) {
                  return point1.x < point2.x;
              });
}

void Graph::draw(QPainter& painter,
                 const QRectF& availableRect,
                 const AxisRange& rangeX,
                 const AxisRange& rangeY)
{
    painter.save();

    populatePointBoxes(availableRect, rangeX, rangeY);
    drawPolyline(painter);
    drawPoints(painter);
    drawAxisLabels(painter, rangeX, availableRect.bottomLeft().y());

    painter.restore();
}

void Graph::populatePointBoxes(const QRectF& availableRect,
                               const AxisRange& rangeX,
                               const AxisRange& rangeY)
{
    const PointBoxScale pointBoxScale{availableRect, rangeX, rangeY};
    std::transform(points.cbegin(),
                   points.cend(),
                   pointBoxes.begin(),
                   point_to_point_box(pointBoxScale));
}

void Graph::drawPolyline(QPainter& painter) const
{
    QPolygonF centerPoints;
    std::transform(pointBoxes.cbegin(),
                   pointBoxes.cend(),
                   std::back_inserter(centerPoints),
                   [](const auto& box) { return box.position; });
    painter.setPen(mPen);
    painter.drawPolyline(centerPoints);
}

void Graph::drawPoints(QPainter& painter)
{
    if (!showPoints())
        return;
    painter.setBrush(pointBoxBrush);
    for (const auto& point : pointBoxes) {
        painter.translate(point.position);
        painter.drawPath(point.path);
        painter.translate(-point.position);
    }
}

void Graph::drawAxisLabels(QPainter& painter,
                           const AxisRange& rangeX,
                           double labelPosY)
{
    if (!showPoints())
        return;
    // For large spans not every label is shown to prevent overlapping.
    const size_t skipLabels{
        std::max(1ul, static_cast<size_t>(rangeX.span()) / labelSkipInd)};

    for (size_t i = 0; i < pointBoxes.size(); i += skipLabels) {
        const auto& point = pointBoxes[i];
        painter.drawText(QPointF{point.position.x(), labelPosY}, point.label);
    }
}

void Graph::handleMouseMoveEvent(QMouseEvent* event) const
{
    if (!showPoints())
        return;

    // According to Qt documentation on should call event->ignore
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

void Graph::setPen(QPen& pen) { mPen = pen; }

const QPen Graph::pen() const { return mPen; }

const GraphPoint& Graph::operator[](size_t idx) const { return points[idx]; }

void Graph::clearData()
{
    points.clear();
    pointBoxes.clear();
}

void Graph::setShowPoints(bool showPoints) { mShowPoints = showPoints; }

bool Graph::showPoints() const { return mShowPoints; }

Graph::const_iterator Graph::cbegin() const { return points.cbegin(); }

Graph::const_iterator Graph::cend() const { return points.cend(); }

size_t Graph::size() const { return points.size(); }

} // namespace sprint_timer::ui::qt_gui

namespace {

PointBoxScale::PointBoxScale(const QRectF& availableRect,
                             const AxisRange& rangeX,
                             const AxisRange& rangeY)
    : labelOffset{labelOffsetRelSize * availableRect.height()}
    , scaleX{rangeX.span() > 0 ? availableRect.width() / rangeX.span() : 1}
    , scaleY{rangeY.span() > 0
                 ? (availableRect.height() - labelOffset) / rangeY.span()
                 : 1}
    , referencePoint{availableRect.bottomLeft()}
    , pbSize{pointBoxRelSize * availableRect.height()}
{
}

QRectF computeAvailableRectange(const QRectF& totalSizeRect)
{
    const double availableWidth{(1 - marginRelSize) * totalSizeRect.width()};
    const double availableHeight{(1 - marginRelSize) * totalSizeRect.height()};
    const QPointF center{totalSizeRect.center()};
    return QRectF{center.x() - availableWidth / 2,
                  center.y() - availableHeight / 2,
                  availableWidth,
                  availableHeight};
}

} // namespace
