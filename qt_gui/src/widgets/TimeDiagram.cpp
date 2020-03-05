/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "qt_gui/widgets/TimeDiagram.h"
#include <QPainter>

namespace {

constexpr double oneMinuteInDegrees{0.25};
constexpr int offsetInDegrees{90};
constexpr int numSegmentsInDegree{16};
constexpr double longTickRelativeLength{0.05};
constexpr double tickAngle{double(360 / 24)};

} // namespace

using dw::DateTimeRange;

namespace sprint_timer::ui::qt_gui {

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
    longTickLength = longTickRelativeLength *
                     std::min(totalSizeRect.width(), totalSizeRect.height());
    shortTickLength = longTickLength / 2;
    tickOffset = shortTickLength;
    diagramRadius = (std::min(totalSizeRect.width(), totalSizeRect.height()) -
                     2 * (longTickLength + tickOffset)) /
                    2;
    diagramRect = QRectF{totalSizeRect.center().x() - diagramRadius,
                         totalSizeRect.center().y() - diagramRadius,
                         diagramRadius * 2,
                         diagramRadius * 2};
}

void TimeDiagram::drawDiagramCanvas(QPainter& painter)
{
    QPen pen;
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
    for (const auto& timeSpan : timeSpans) {
        double start = (timeSpan.start().hour().count() * 60 +
                        timeSpan.start().minute().count()) *
                       oneMinuteInDegrees;
        const double span = timeSpan.duration<std::chrono::minutes>().count() *
                            oneMinuteInDegrees;
        painter.drawPie(diagramRect,
                        -(int(start) - offsetInDegrees) * numSegmentsInDegree,
                        -int(span) * numSegmentsInDegree);
    }
}

void TimeDiagram::setIntervals(std::vector<dw::DateTimeRange> newIntervals)
{
    timeSpans = std::move(newIntervals);
    update();
}

} // namespace sprint_timer::ui::qt_gui

