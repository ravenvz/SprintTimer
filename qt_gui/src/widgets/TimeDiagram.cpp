/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#include "TimeDiagram.h"
#include <QPainter>


TimeDiagram::TimeDiagram(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
}

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
    totalSizeRect = rect();
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
    int sprintDuration = 25;
    int numSegmentsInDegree = 16;
    const auto& timeSpansRef = timeSpans;
    for (const TimeSpan& timeSpan : timeSpansRef) {
        double start
            = (timeSpan.start().hour() * 60 + timeSpan.start().minute())
            * oneMinuteInDegrees;
        // TODO replace with stored sprint duration when implemented
        double span = sprintDuration * oneMinuteInDegrees;
        painter.drawPie(diagramRect,
                        -(int(start) - offsetInDegrees) * numSegmentsInDegree,
                        -int(span) * numSegmentsInDegree);
    }
}

void TimeDiagram::setIntervals(std::vector<TimeSpan> newIntervals)
{
    timeSpans = std::move(newIntervals);
    update();
}
