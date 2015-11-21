#include <algorithm>
#include "plot.h"

#include <QDebug>

Graph::Graph() {

}

void Graph::setData(GraphData& data) {
    points = data;
    std::sort(points.begin(), points.end(), [](auto& point1, auto& point2) {
            return point1.x < point2.x;
        });
    // for (auto& point : points) {
    //     qDebug() << "(" << point.x << ", " << point.y << ")";
    // }
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


Plot::Plot(QWidget* parent) :
    QWidget(parent)
{
}

void Plot::addGraph(Graph& graph) {
    graphs.push_back(graph);
}


void Plot::setGraphData(int graphNum, GraphData& data) {
    if (0 <= graphNum && graphNum < graphs.size()) {
        graphs[graphNum].setData(data);
    }
}

void Plot::replot() {
    this->repaint();
}

void Plot::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    computeAdaptiveSizes();
    // Initial call to paintEvent could be before graphs are set
    if (graphs.empty() || graphs[0].size() < 2) {
        painter.eraseRect(availableRect);
        return;
    }
    double maxHeight = availableRect.height();
    double maxWidth = availableRect.width();
    double scaleX = maxWidth / (rangeX.getSpan() - 1);
    double scaleY = maxHeight / (rangeY.getSpan() - 1);
    qDebug() << availableRect;
    qDebug() << "Scale X" << scaleX;
    qDebug() << "Scale Y" << scaleY;
    // QPointF currentZeroPoint = availableRect.bottomLeft();
    // QPen bigRed;
    // bigRed.setWidthF(2.2);
    // bigRed.setColor(Qt::red);
    // painter.setPen(bigRed);

    for (auto& graph : graphs) {
        painter.setPen(graph.getPen());
        QPointF referencePoint = availableRect.bottomLeft();
        QVector<QPointF> points;
        std::transform(graph.cbegin(), graph.cend(), std::back_inserter(points), [scaleX, scaleY, referencePoint](auto& p) {
                return QPointF(p.x * scaleX + referencePoint.x(), referencePoint.y() - p.y * scaleY);
            });
        QPainterPath path;
        for (int i = 0; i < points.size(); ++i) {
            if (i < points.size() - 1) { 
                painter.drawLine(points[i], points[i + 1]);
            }
            painter.setBrush(Qt::red);
            painter.drawEllipse(points[i], 0.025 * maxHeight, 0.025 * maxHeight);
            painter.setBrush(Qt::white);
            painter.drawEllipse(points[i], 0.02 * maxHeight, 0.02 * maxHeight);
        }
        painter.drawPath(path);
        points.clear();
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
}
