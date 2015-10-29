#include <QtGui/qpainter.h>
#include "piediagram.h"
#include <algorithm>

#include <QDebug>

constexpr double pi() {
    return acos(-1);
}

PieDiagram::PieDiagram(QWidget* parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    connectSlots();
}

void PieDiagram::connectSlots() {
    connect(this, SIGNAL(sliceSelectionChanged(int)), this, SLOT(onSliceSelectionChanged(int)));
}

void PieDiagram::setData(QVector<Slice> data) {
    selectedPieIndex = -1;
    sortedData.clear();
    std::transform(data.begin(), data.end(), std::back_inserter(sortedData),
        [](auto entry) {
            return std::make_pair(entry.first, entry.second * 360);
        });
    repaint();
}

void PieDiagram::paintEvent(QPaintEvent* event) {
    computeAdaptiveSizes();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(Qt::black);
    painter.setPen(pen);
//    painter.drawEllipse(diagramRect);
//    painter.drawEllipse(expandedSliceRect);

    QVector<QBrush> brushes {QBrush(Qt::red),
                             QBrush(Qt::magenta),
                             QBrush(Qt::darkGreen),
                             QBrush(Qt::cyan),
                             QBrush(Qt::darkCyan),
                             QBrush(Qt::blue)};

    painter.setPen(QPen(Qt::gray));

    double offset = 0;
    for (int i = 0; i < sortedData.size(); ++i) {
        painter.setBrush(brushes[i % brushes.size()]);
        if (i == selectedPieIndex) {
            QPointF offsetPoint = computeOffsetPoint(sortedData[i].second, offset);
            painter.translate(offsetPoint);
            painter.drawPie(diagramRect, int(offset * 16), int(sortedData[i].second * 16));
            painter.resetTransform();
        } else {
            painter.drawPie(diagramRect, int(offset * 16), int(sortedData[i].second * 16));
        }
        offset += sortedData[i].second;
    }
}

QPointF PieDiagram::computeOffsetPoint(double current, double offset) {
    double angle = offset + current / 2;
    qDebug() << "Angle " << angle;
    double angleRads = angle * pi() / 180;
    double x = expandedShiftLength * cos(angleRads);
    double y = expandedShiftLength * sin(angleRads);
    if (0 <= angle && angle <= 90) y = -y;
    else if (90 < angle && angle <= 180) y = -y;
    else if (180 < angle && angle <= 270) y = -y;
    else y = -y;
    return QPointF {x, y};
}

void PieDiagram::computeAdaptiveSizes() {
    totalSizeRect = QRectF(QPointF(0, 0), this->size());
    QPointF center = totalSizeRect.center();
    double expandedSliceRelativeDiameter = 0.98 * std::min(totalSizeRect.width(), totalSizeRect.height());
    double diagramRelativeDiameter = expandedSliceRelativeDiameter - 0.1*expandedSliceRelativeDiameter;
    expandedShiftLength = (expandedSliceRelativeDiameter -  diagramRelativeDiameter) / 2;
    expandedSliceRect = QRectF {center.x() - expandedSliceRelativeDiameter / 2,
                                center.y() - expandedSliceRelativeDiameter / 2,
                                expandedSliceRelativeDiameter,
                                expandedSliceRelativeDiameter};
    diagramRect = QRectF {center.x() - diagramRelativeDiameter / 2,
                          center.y() - diagramRelativeDiameter / 2,
                          diagramRelativeDiameter,
                          diagramRelativeDiameter};

}

void PieDiagram::mousePressEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton) {
        updateSelectedSliceIndex(event->pos());
    }
}

void PieDiagram::updateSelectedSliceIndex(const QPoint& pos) {
    double angle = QLineF(diagramRect.center(), pos).angle();
    double offset = 0;
    for (int i = 0; i < sortedData.size(); ++i) {
        offset += sortedData[i].second;
        if (angle < offset) {
            emit sliceSelectionChanged(i);
            break;
        }
    }
}

void PieDiagram::onSliceSelectionChanged(int sliceIndex) {
    selectedPieIndex = selectedPieIndex == sliceIndex ? -1 : sliceIndex;
    repaint();
}

