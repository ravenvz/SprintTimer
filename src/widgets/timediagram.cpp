#include "timediagram.h"
#include <QPainter>


TimeDiagram::TimeDiagram(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
}

TimeDiagram::~TimeDiagram() {}

void TimeDiagram::paintEvent(QPaintEvent*)
{
    computeAdaptiveSizes();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawDiagramCanvas(painter);
    drawIntervals(painter);
}

void TimeDiagram::computeAdaptiveSizes()
{
    totalSizeRect = QRectF(QPointF(0, 0), this->size());
    double longTickRelativeLength = 0.05;
    longTickLength = longTickRelativeLength
        * std::min(totalSizeRect.width(), totalSizeRect.height());
    shortTickLength = longTickLength / 2;
    tickOffset = shortTickLength;
    diagramRadius = (std::min(totalSizeRect.width(), totalSizeRect.height())
                     - 2 * (longTickLength + tickOffset))
        / 2;
    diagramRect = QRectF{totalSizeRect.center().x() - diagramRadius,
                         totalSizeRect.center().y() - diagramRadius,
                         diagramRadius * 2,
                         diagramRadius * 2};
}

void TimeDiagram::drawDiagramCanvas(QPainter& painter)
{
    QPen pen;
    double tickAngle = double(360 / 24);
    pen.setWidthF(1.2);
    pen.setColor(Qt::gray);
    painter.setPen(pen);
    painter.drawEllipse(diagramRect);
    QPointF center = diagramRect.center();
    for (size_t i = 0; i < 6; ++i) {
        double length = i % 6 == 0 ? longTickLength : shortTickLength;
        painter.drawLine(
            QPointF(center.x(), center.y() - diagramRadius - tickOffset),
            QPointF(center.x(),
                    center.y() - diagramRadius - tickOffset - length));
        painter.drawLine(
            QPointF(center.x(), center.y() + diagramRadius + tickOffset),
            QPointF(center.x(),
                    center.y() + diagramRadius + tickOffset + length));
        painter.drawLine(
            QPointF(center.x() - diagramRadius - tickOffset, center.y()),
            QPointF(center.x() - diagramRadius - tickOffset - length,
                    center.y()));
        painter.drawLine(
            QPointF(center.x() + diagramRadius + tickOffset, center.y()),
            QPointF(center.x() + diagramRadius + tickOffset + length,
                    center.y()));
        painter.translate(center);
        painter.rotate(tickAngle);
        painter.translate(-center);
    }
    painter.resetTransform();
}

void TimeDiagram::drawIntervals(QPainter& painter)
{
    QBrush arcBrush = QBrush(timeSpanColor);
    painter.setBrush(arcBrush);
    painter.setPen(Qt::NoPen);
    double oneMinuteInDegrees = 0.25;
    int offsetInDegrees = 90;
    int pomoDurationInMinutes = 25;
    int numSegmentsInDegree = 16;
    const auto& intervalsRef = intervals;
    for (const TimeInterval& interval : intervalsRef) {
        double start
            = (interval.startTime.hour() * 60 + interval.startTime.minute())
            * oneMinuteInDegrees;
        // TODO replace with stored pomodoro duration when implemented
        double span = pomoDurationInMinutes * oneMinuteInDegrees;
        painter.drawPie(diagramRect,
                        -(int(start) - offsetInDegrees) * numSegmentsInDegree,
                        -int(span) * numSegmentsInDegree);
    }
}

void TimeDiagram::setIntervals(QVector<TimeInterval> newIntervals)
{
    intervals = newIntervals;
    update();
}
