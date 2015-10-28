#include <QtGui/qpainter.h>
#include "piediagram.h"
#include <algorithm>

#include <QDebug>

constexpr double pi() {
    return acos(-1);
}

PieDiagram::PieDiagram(int numSlices, QWidget* parent) :
    QWidget(parent),
    numSlices(numSlices)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
}

void PieDiagram::setData(QHash<QString, unsigned> data) {
    dataHash = data;
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

    QVector<double> angles = computeAngles();

    QVector<QBrush> brushes {QBrush(Qt::red),
                             QBrush(Qt::magenta),
                             QBrush(Qt::darkGreen),
                             QBrush(Qt::cyan),
                             QBrush(Qt::darkCyan),
                             QBrush(Qt::blue)};

    painter.setPen(QPen(Qt::gray));

    double offset = 0;
    for (int i = 0; i < std::min(dataHash.size(), numSlices); ++i) {
//        double current = (double(sortedValues[i].first)/total)*360;
        painter.setBrush(brushes[i % brushes.size()]);
        if (i == 4) {
            QPointF offsetPoint = computeOffsetPoint(angles[i], offset);
            painter.translate(offsetPoint);
            painter.drawPie(diagramRect, int(offset) * 16, int(angles[i] * 16));
            painter.resetTransform();
        } else {
            painter.drawPie(diagramRect, int(offset) * 16, int(angles[i] * 16));
        }
        offset += angles[i];
    }
}

QVector<double> PieDiagram::computeAngles() const {
    unsigned total = 0;
    QVector<std::pair<unsigned, QString> > sortedValues;
    QHash<QString, unsigned>::const_iterator it;
    for (it = dataHash.begin(); it != dataHash.end(); ++it) {
        sortedValues.push_back(std::make_pair(it.value(), it.key()));
        total += it.value();
    }
    sort(sortedValues.begin(), sortedValues.end(), [](auto a, auto b) { return a > b; });
    QVector<double> angles;
    transform(sortedValues.begin(), sortedValues.end(), std::back_inserter(angles),
        [total](auto entry) {
            return double(entry.first)/total * 360;
        });
    qDebug() << angles;
    while (angles.size() > numSlices) {
        angles[angles.size() - 2] += angles[angles.size() - 1];
        angles.pop_back();
    }
    qDebug() << angles;
    return angles;
}

QPointF PieDiagram::computeOffsetPoint(double current, double offset) {
    double angle = offset + current / 2;
    qDebug() << "Angle " << angle;
    double angleRads = angle * pi() / 180;
    double newX = expandedShiftLength * cos(angleRads);
    double newY = expandedShiftLength * sin(angleRads);
    if (0 <= angle && angle <= 90) newY = -newY;
    else if (90 < angle && angle <= 180) newY = -newY;
    else if (180 < angle && angle <= 270) newY = -newY;
    else newY = -newY;
    return QPointF {newX, newY};
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
