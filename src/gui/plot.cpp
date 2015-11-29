#include <algorithm>
#include "plot.h"
#include <QHelpEvent>
#include <QToolTip>

#include <QDebug>


Graph::Graph() {

}

void Graph::setData(GraphData& data) {
    points = data;
    std::sort(points.begin(), points.end(), [](const auto& point1, const auto& point2) {
            return point1.x < point2.x;
        });
}

void Graph::clearData() {
    points.clear();
}

void Graph::setPen(QPen& pen) {
    this->pen = pen;
}

const QPen Graph::getPen() const {
    return pen;
}

const GraphPoint& Graph::operator[](std::size_t idx) const {
    return points[idx];
}

size_t Graph::size() const {
    return points.size();
}

void Graph::setShowPoints(bool showPoints) {
    mShowPoints = showPoints;
}

bool Graph::showPoints() const {
    return mShowPoints;
}


Plot::Plot(QWidget* parent) :
    QWidget(parent)
{
    setMouseTracking(true);
    connect(this, SIGNAL(sizeComputed()), this, SLOT(onSizeComputed()));
}

void Plot::addGraph(Graph& graph) {
    graphs.push_back(graph);
    pointBoxes.push_back(PointBoxContainer {});
}

void Plot::setGraphData(int graphNum, GraphData& data) {
    if (graphNum < 0 || graphNum >= graphs.size()) {
        return;
    }
    graphs[graphNum].setData(data);
    adaptiveSizeComputed = false;
}

void Plot::constructPointBoxes() {
    const double labelOffset = labelOffsetRelSize * availableRect.height();
    const double scaleX = availableRect.width() / (rangeX.getSpan() - 1);
    const double scaleY = (availableRect.height() - labelOffset) / (rangeY.getSpan() - 1);
    for (int graphNum = 0; graphNum < graphs.size(); ++graphNum) {
        QPointF referencePoint = availableRect.bottomLeft();
        pointBoxes[graphNum].clear();
        std::transform(graphs[graphNum].cbegin(), graphs[graphNum].cend(),
                       std::back_inserter(pointBoxes[graphNum]),
                       [pointBoxSize = pointBoxSize, scaleX, scaleY, referencePoint, labelOffset](const auto& p)
            {
                QPainterPath path;
                QPointF position {p.x * scaleX + referencePoint.x(),
                                  referencePoint.y() - p.y * scaleY - labelOffset};
                path.addEllipse(QPointF {0, 0}, pointBoxSize, pointBoxSize);
                return PointBox {path,
                                 position,
                                 QString("%1").arg(p.y),
                                 p.label};
            });
    }
}

void Plot::replot() {
    repaint();
}

void Plot::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (!adaptiveSizeComputed)
        computeAdaptiveSizes();
    if (pointBoxes.empty()) {
        painter.eraseRect(availableRect);
        return;
    }

    // Only labelSkip-th labels will be shown, so that labels are not cluttered with large span.
    const int labelSkip {int(rangeX.getSpan() - 1) / labelSkipInd};

    for (int graphNum = 0; graphNum < graphs.size(); ++graphNum) {
        const auto& graph {graphs[graphNum]};
        const auto& boxes {pointBoxes[graphNum]};
        QPolygonF centerPoints;
        std::transform(boxes.cbegin(),
                       boxes.cend(),
                       std::back_inserter(centerPoints),
                       [](const auto& box) { return box.position; });

        painter.setPen(graph.getPen());
        painter.drawPolyline(centerPoints);
        if (!graph.showPoints())
            continue;
        for (int i = 0; i < boxes.size(); ++i) {
            const auto& point = boxes[i];
            painter.setBrush(pointBoxBrush);
            painter.translate(point.position);
            painter.drawPath(point.path);
            painter.translate(-point.position);
            if (i % labelSkip == 0)
                painter.drawText(point.position.x(), availableRect.bottomLeft().y(), point.label);
        }
    }
}

void Plot::setRangeX(double start, double end) {
    rangeX.setRange(start, end);
}

void Plot::setRangeY(double start, double end) {
    rangeY.setRange(start, end);
}

void Plot::computeAdaptiveSizes() {
    QRectF totalSizeRect {QRectF(QPointF(0, 0), this->size())};
    const QPointF center {totalSizeRect.center()};
    const double availableWidth {(1 - marginRelSize) * totalSizeRect.width()};
    const double availableHeight {(1 - marginRelSize) * totalSizeRect.height()};
    availableRect = {QRectF {center.x() - availableWidth / 2,
                             center.y() - availableHeight / 2,
                             availableWidth,
                             availableHeight}};
    pointBoxSize = {pointBoxRelSize * availableRect.height()};
    emit sizeComputed();
}

void Plot::reset() {
    for (auto& points : pointBoxes) {
        points.clear();
    }
    for (auto& graph : graphs) {
        graph.clearData();
    }
}

void Plot::onSizeComputed() {
    constructPointBoxes();
    adaptiveSizeComputed = true;
}

void Plot::mouseMoveEvent(QMouseEvent* event) {
    const auto& toolTip = getPosTooltip(event->pos());
    if (toolTip.size() != 0) {
        const QPoint toolTipPos {event->globalPos() - QPoint{int(pointBoxSize),
                                 toolTipOffset * int(pointBoxSize)}};
        QToolTip::showText(toolTipPos, toolTip);
    } else {
        QToolTip::hideText();
        event->ignore();
    }
}

const QString Plot::getPosTooltip(const QPoint& pos) const {
    for (int graphNum = 0; graphNum < graphs.size(); ++graphNum) {
        if (!graphs[graphNum].showPoints())
            continue;
        for (const auto& box : pointBoxes[graphNum]) {
            if (box.path.contains(pos - box.position)) {
                return box.toolTip;
            }
        }
    }
    return {""};
}
