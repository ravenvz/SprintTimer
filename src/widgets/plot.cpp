#include <algorithm>
#include "plot.h"
#include <QHelpEvent>
#include <QToolTip>

#include <QDebug>

void AxisRange::setRange(double start, double end)
{
    this->start = std::min(start, end);
    this->end = std::max(start, end);
}

double AxisRange::span() const { return end - start; }

Plot::Plot(QWidget* parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    connect(this, SIGNAL(sizeComputed()), this, SLOT(onSizeComputed()));
}

void Plot::addGraph(Graph& graph)
{
    graphs.push_back(graph);
    pointBoxes.push_back(PointBoxContainer{});
}

void Plot::setGraphData(int graphNum, GraphData& data)
{
    if (graphNum < 0 || graphNum >= graphs.size()) {
        return;
    }
    graphs[graphNum].setData(data);
    adaptiveSizeComputed = false;
}

void Plot::reset()
{
    for (auto& points : pointBoxes) {
        points.clear();
    }
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
    if (!adaptiveSizeComputed)
        computeAdaptiveSizes();
    if (pointBoxes.empty()) {
        painter.eraseRect(availableRect);
        return;
    }

    for (int graphNum = 0; graphNum < graphs.size(); ++graphNum) {
        paintGraph(graphNum, painter);
    }
}

void Plot::mouseMoveEvent(QMouseEvent* event)
{
    const auto& toolTip = getPosTooltip(event->pos());
    if (toolTip.size() != 0) {
        const QPoint toolTipPos{
            event->globalPos()
            - QPoint{int(pointBoxSize), toolTipOffset * int(pointBoxSize)}};
        QToolTip::showText(toolTipPos, toolTip);
    }
    else {
        QToolTip::hideText();
        event->ignore();
    }
}

void Plot::onSizeComputed()
{
    constructPointBoxes();
    adaptiveSizeComputed = true;
}

void Plot::computeAdaptiveSizes()
{
    QRectF totalSizeRect{QRectF(QPointF(0, 0), this->size())};
    const QPointF center{totalSizeRect.center()};
    const double availableWidth{(1 - marginRelSize) * totalSizeRect.width()};
    const double availableHeight{(1 - marginRelSize) * totalSizeRect.height()};
    availableRect = {QRectF{center.x() - availableWidth / 2,
                            center.y() - availableHeight / 2,
                            availableWidth,
                            availableHeight}};
    pointBoxSize = {pointBoxRelSize * availableRect.height()};
    emit sizeComputed();
}

void Plot::constructPointBoxes()
{
    const double labelOffset = labelOffsetRelSize * availableRect.height();
    const double scaleX
        = rangeX.span() > 0 ? availableRect.width() / (rangeX.span()) : 1;
    const double scaleY = rangeY.span() > 0
        ? (availableRect.height() - labelOffset) / (rangeY.span())
        : 1;
    for (int graphNum = 0; graphNum < graphs.size(); ++graphNum) {
        QPointF referencePoint = availableRect.bottomLeft();
        pointBoxes[graphNum].clear();
        std::transform(
            graphs[graphNum].cbegin(),
            graphs[graphNum].cend(),
            std::back_inserter(pointBoxes[graphNum]),
            [
              pointBoxSize = pointBoxSize,
              scaleX,
              scaleY,
              referencePoint,
              labelOffset
            ](const auto& p) {
                QPainterPath path;
                QPointF position{p.x * scaleX + referencePoint.x(),
                                 referencePoint.y() - p.y * scaleY
                                     - labelOffset};
                path.addEllipse(QPointF{0, 0}, pointBoxSize, pointBoxSize);
                return PointBox{
                    path, position, QString("%1").arg(p.y), p.label};
            });
    }
}

void Plot::paintGraph(const int graphNum, QPainter& painter) const
{
    const auto& graph = graphs[graphNum];
    const auto& boxes = pointBoxes[graphNum];
    QPolygonF centerPoints;
    std::transform(boxes.cbegin(),
                   boxes.cend(),
                   std::back_inserter(centerPoints),
                   [](const auto& box) { return box.position; });

    painter.setPen(graph.pen());
    painter.drawPolyline(centerPoints);
    if (graph.showPoints()) {
        paintPoints(boxes, painter);
    }
}

void Plot::paintPoints(const PointBoxContainer& boxes, QPainter& painter) const
{
    // Only labelSkip-th labels will be shown, so that labels are not cluttered
    // with large span.
    int labelSkip{static_cast<int>(rangeX.span()) / labelSkipInd};
    if (labelSkip < 1)
        labelSkip = 1;

    for (int i = 0; i < boxes.size(); ++i) {
        const auto& point = boxes[i];
        painter.setBrush(pointBoxBrush);
        painter.translate(point.position);
        painter.drawPath(point.path);
        painter.translate(-point.position);
        if (i % labelSkip == 0)
            painter.drawText(
                QPointF{point.position.x(), availableRect.bottomLeft().y()},
                point.label);
    }
}

const QString Plot::getPosTooltip(const QPoint& pos) const
{
    for (int graphNum = 0; graphNum < graphs.size(); ++graphNum) {
        if (!graphs[graphNum].showPoints())
            continue;
        const auto& pointBoxesGraphRef = pointBoxes[graphNum];
        for (const auto& box : pointBoxesGraphRef) {
            if (box.path.contains(pos - box.position)) {
                return box.toolTip;
            }
        }
    }
    return {""};
}

void Graph::setData(GraphData& data)
{
    points = data;
    std::sort(points.begin(),
              points.end(),
              [](const auto& point1, const auto& point2) {
                  return point1.x < point2.x;
              });
}

void Graph::setPen(QPen& pen) { mPen = pen; }

const QPen Graph::pen() const { return mPen; }

const GraphPoint& Graph::operator[](int idx) const { return points[idx]; }

void Graph::clearData() { points.clear(); }

void Graph::setShowPoints(bool showPoints) { mShowPoints = showPoints; }

bool Graph::showPoints() const { return mShowPoints; }

Graph::const_iterator Graph::cbegin() const { return points.cbegin(); }

Graph::const_iterator Graph::cend() const { return points.cend(); }

int Graph::size() const { return points.size(); }
