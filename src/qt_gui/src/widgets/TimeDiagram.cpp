/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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

constexpr int32_t degreesInCircle{360};
constexpr int32_t numSegmentsInDegree{16};
constexpr int32_t minutesInDay{24 * 60};
constexpr int32_t oneDegreeInMinutes{minutesInDay / degreesInCircle};
constexpr int offsetInDegrees{90};
constexpr double longTickRelativeLength{0.05};
constexpr int32_t tickAngle{degreesInCircle / 24};
constexpr size_t numTicksPerQuarter{6};
constexpr double dialPenWidth{1.2};

constexpr auto minutesFromMidnight(const auto& dateTime) noexcept -> int32_t;

constexpr auto
mapDateTimeToSegmentedAngle(const dw::DateTime& dateTime) noexcept -> int32_t;

constexpr auto timeSpanToArc(const dw::DateTimeRange& timeSpan) noexcept
    -> std::pair<int, int>;

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
    drawingParams = DrawingParams{rect()};
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawDiagramCanvas(painter);
    drawIntervals(painter);
}

void TimeDiagram::drawDiagramCanvas(QPainter& painter)
{
    QPen pen;
    pen.setWidthF(dialPenWidth);
    pen.setColor(Qt::gray);
    painter.setPen(pen);
    const auto [diagramRect,
                diagramRadius,
                tickOffset,
                longTickLength,
                shortTickLength] = drawingParams;
    painter.drawEllipse(diagramRect);
    QPointF center = diagramRect.center();
    // Literally, we drawing just drawing 4 ticks (by 2 that are opposite to
    // each other) and rotating coordinate system (by 45 degrees in total) so
    // that all are drawn
    for (size_t i = 0; i < numTicksPerQuarter; ++i) {
        const bool bigTick{i % numTicksPerQuarter == 0};
        const double length = bigTick ? longTickLength : shortTickLength;
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
    const QBrush arcBrush = QBrush(timeSpanColor);
    painter.setBrush(arcBrush);
    painter.setPen(Qt::NoPen);

    for (const auto& timeSpan : timeSpans) {
        const auto [startAngle, spanAngle] = timeSpanToArc(timeSpan);
        painter.drawPie(drawingParams.diagramRect, startAngle, spanAngle);
    }
}

void TimeDiagram::setIntervals(std::vector<dw::DateTimeRange> newIntervals)
{
    timeSpans = std::move(newIntervals);
    update();
}

TimeDiagram::DrawingParams::DrawingParams(const QRectF& totalSizeRect)
{
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

} // namespace sprint_timer::ui::qt_gui

namespace {

constexpr auto minutesFromMidnight(const auto& dateTime) noexcept -> int32_t
{
    return static_cast<int32_t>((dateTime.hour() + dateTime.minute()).count());
}

constexpr auto
mapDateTimeToSegmentedAngle(const dw::DateTime& dateTime) noexcept -> int32_t
{
    const auto angle = minutesFromMidnight(dateTime) / oneDegreeInMinutes;
    return -(angle - offsetInDegrees) * numSegmentsInDegree;
}

constexpr auto timeSpanToArc(const dw::DateTimeRange& timeSpan) noexcept
    -> std::pair<int, int>
{
    const auto startAngle = mapDateTimeToSegmentedAngle(timeSpan.start());
    const auto stopAngle = mapDateTimeToSegmentedAngle(timeSpan.finish());
    const auto spanAngle = stopAngle - startAngle;
    return {startAngle, spanAngle};
}

} // namespace
