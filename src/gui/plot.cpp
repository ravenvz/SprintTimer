#include <algorithm>
#include "plot.h"
#include <QHelpEvent>
#include <QToolTip>

#include <QDebug>


QPainterPath PointBox::path() const
{
    return mPath;
}

QPoint PointBox::position() const
{
    return mPosition;
}

QColor PointBox::color() const
{
    return mColor;
}

QString PointBox::toolTip() const
{
    return mToolTip;
}

void PointBox::setPath(const QPainterPath &path)
{
    mPath = path;
}

void PointBox::setToolTip(const QString &toolTip)
{
    mToolTip = toolTip;
}

void PointBox::setPosition(const QPoint &position)
{
    mPosition = position;
}

void PointBox::setColor(const QColor &color)
{
    mColor = color;
}

Graph::Graph() {

}

void Graph::setData(GraphData& data) {
    points = data;
    std::sort(points.begin(), points.end(), [](auto& point1, auto& point2) {
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
    translatedPoints.push_back(PointPixelCoordinates());

}

void Plot::setGraphData(int graphNum, GraphData& data) {
    if (graphNum < 0 || graphNum >= graphs.size()) {
        return;
    }
    graphs[graphNum].setData(data);
    adaptiveSizeComputed = false;
}

void Plot::constructPointBoxes() {
    const double labelOffset = 0.15 * availableRect.height();
    const double scaleX = availableRect.width() / (rangeX.getSpan() - 1);
    const double scaleY = (availableRect.height() - labelOffset) / (rangeY.getSpan() - 1);
    for (int graphNum = 0; graphNum < graphs.size(); ++graphNum) {
        QPointF referencePoint = availableRect.bottomLeft();
        translatedPoints[graphNum].clear();
        std::transform(graphs[graphNum].cbegin(), graphs[graphNum].cend(),
                       std::back_inserter(translatedPoints[graphNum]),
                       [scaleX, scaleY, referencePoint, labelOffset](auto& p) {
               return QPointF {p.x * scaleX + referencePoint.x(),
                               referencePoint.y() - p.y * scaleY - labelOffset};
               });
    }
}

void Plot::replot() {
    this->repaint();
}

void Plot::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (!adaptiveSizeComputed)
        computeAdaptiveSizes();
    if (translatedPoints.empty()) {
        painter.eraseRect(availableRect);
        return;
    }

    // Skip each labelSkip label so that labels are not cluttered.
    const int labelSkip = (rangeX.getSpan() - 1) / 28;
    const double innerRelSize = 0.8 * pointBoxSize;
    int graphNum = 0;
    for (auto& graph : graphs) {
        const QPolygonF points = translatedPoints[graphNum++];
        painter.setPen(graph.getPen());
        painter.drawPolyline((QPolygonF) points);
        if (!graph.showPoints())
            continue;
        for (int i = 0; i < points.size(); ++i) {
            painter.setBrush(Qt::red);
            painter.drawEllipse(points[i], pointBoxSize, pointBoxSize);
            painter.setBrush(Qt::white);
            painter.drawEllipse(points[i], innerRelSize * pointBoxSize, innerRelSize * pointBoxSize);
            if (i % labelSkip == 0)
                painter.drawText(points[i].x(), availableRect.bottomLeft().y(), graph[i].label);
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
    QRectF totalSizeRect = QRectF(QPointF(0, 0), this->size());
    const QPointF center = totalSizeRect.center();
    const double margin = 0.07;
    const double availableWidth = (1 - margin) * totalSizeRect.width();
    const double availableHeight = (1 - margin) * totalSizeRect.height();
    availableRect = QRectF {center.x() - availableWidth / 2,
                            center.y() - availableHeight / 2,
                            availableWidth,
                            availableHeight};
    pointBoxSize = 0.0125 * availableRect.height();
    emit sizeComputed();
}

void Plot::reset() {
    for(auto& points : translatedPoints) {
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
    auto index = pointBoxContain(event->pos());
    if (index.first != -1) {
        QString toolTip = QString("%1").arg(graphs[index.first][index.second].y);
        QToolTip::showText(event->globalPos() - QPoint {0, int(15 * pointBoxSize)}, toolTip);
    } else {
        QToolTip::hideText();
        event->ignore();
    }
}

std::pair<int, int> Plot::pointBoxContain(const QPoint& pos) const {
    for (int graphNum = 0; graphNum < graphs.size(); ++graphNum) {
        if (!graphs[graphNum].showPoints())
            continue;
        for (int i = 0; i < translatedPoints[graphNum].size(); ++i) {
            QRectF pointBox {translatedPoints[graphNum][i].x() - pointBoxSize,
                             translatedPoints[graphNum][i].y() - pointBoxSize,
                             pointBoxSize,
                             pointBoxSize};
            if (pointBox.contains(pos)) {
                return std::make_pair(graphNum, i);
            }
        }
    }
    return std::make_pair(-1, -1);
}
