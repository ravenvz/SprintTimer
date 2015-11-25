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


Plot::Plot(QWidget* parent) :
    QWidget(parent)
{
    connect(this, SIGNAL(sizeComputed()), this, SLOT(onSizeComputed()));
}

void Plot::addGraph(Graph& graph) {
    graphs.push_back(graph);
    graphPointBoxes.push_back(PointBoxData());
}


void Plot::setGraphData(int graphNum, GraphData& data) {
    if (graphNum < 0 || graphNum >= graphs.size()) {
        return;
    }
    graphs[graphNum].setData(data);
    adaptiveSizeComputed = false;
}

void Plot::constructPointBoxes() {
    const double maxHeight = availableRect.height();
    const double maxWidth = availableRect.width();
    const double scaleX = maxWidth / (rangeX.getSpan() - 1);
    const double scaleY = maxHeight / (rangeY.getSpan() - 1);
    const double pointBoxSize = 0.025 * maxHeight;
    for (int graphNum = 0; graphNum < graphs.size(); ++graphNum) {
        QPointF referencePoint = availableRect.bottomLeft();
        graphPointBoxes[graphNum].clear();
        std::transform(graphs[graphNum].cbegin(), graphs[graphNum].cend(),
                       std::back_inserter(graphPointBoxes[graphNum]),
                       [scaleX, scaleY, referencePoint, pointBoxSize](auto& p) {
                return QRectF {p.x * scaleX + referencePoint.x() - pointBoxSize / 2,
                               referencePoint.y() - p.y * scaleY - pointBoxSize / 2,
                               pointBoxSize,
                               pointBoxSize};
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
    if (graphPointBoxes.empty()) {
        painter.eraseRect(availableRect);
        return;
    }

    int graphNum = 0;
    for (auto& graph : graphs) {
        painter.setPen(graph.getPen());
        PointBoxData pointBoxes = graphPointBoxes[graphNum++];
        for (int i = 0; i < pointBoxes.size(); ++i) {
            if (i < pointBoxes.size() - 1) {
                painter.drawLine(pointBoxes[i].center(), pointBoxes[i + 1].center());
            }
            QRectF currentBox = pointBoxes[i];
            painter.setBrush(Qt::red);
            painter.drawEllipse(currentBox.center(), currentBox.width(), currentBox.height());
            painter.setBrush(Qt::white);
            painter.drawEllipse(currentBox.center(), 0.8 * currentBox.width(), 0.8 * currentBox.height());
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
    emit sizeComputed();
}

void Plot::reset() {
    for(auto& pointBoxes : graphPointBoxes) {
        pointBoxes.clear();
    }
    for (auto& graph : graphs) {
        graph.clearData();
    }
}

void Plot::showEvent(QShowEvent*) {
    qDebug() << "Here we go";
    this->repaint();
}

void Plot::onSizeComputed() {
    constructPointBoxes();
    adaptiveSizeComputed = true;
    qDebug() << "Signal received";
}
