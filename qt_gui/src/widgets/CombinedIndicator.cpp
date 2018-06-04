/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "widgets/CombinedIndicator.h"
#include <QPainter>
#include <cmath>

double degreesToRadians(int degrees);

CombinedIndicator::CombinedIndicator(int sideSizeHint, QWidget* parent)
    : widthAndHeightHint{sideSizeHint}
    , QWidget{parent}
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

QSize CombinedIndicator::sizeHint() const
{
    return QSize(widthAndHeightHint, widthAndHeightHint);
}

void CombinedIndicator::setMaxValue(int value) { maxValue = value; }

void CombinedIndicator::setCurrentValue(int value) { currentValue = value; }

void CombinedIndicator::setText(QString&& text)
{
    currentText = std::move(text);
}

void CombinedIndicator::setInvertedStyle(bool inverted)
{
    invertedDrawStyle = inverted;
}

void CombinedIndicator::setDrawArc(bool shouldDraw) { drawArc = shouldDraw; }

void CombinedIndicator::setColor(const QColor& color) { this->color = color; }

void CombinedIndicator::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawText(painter);
    if (drawArc)
        drawProgressArc(painter);
}

void CombinedIndicator::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit indicatorClicked();
    }
}

void CombinedIndicator::drawProgressArc(QPainter& painter) const
{
    constexpr int qtDegPrecision{16};
    constexpr double marginRatio{0.05};
    constexpr int startAngle{90};
    constexpr int pointRadius{4};
    constexpr int pointInnerRadius{3};
    constexpr double penSize{4.0};

    const int margin = static_cast<int>(rect().width() * marginRatio);
    const QRectF boundingRect
        = rect().adjusted(margin, margin, -margin, -margin);

    // Draw arc.
    QPen progressPen{color};
    progressPen.setWidthF(penSize);
    painter.setPen(progressPen);
    double tickSize{360 / double(maxValue)};
    double spanAngle = (maxValue - currentValue) * tickSize;
    if (invertedDrawStyle)
        spanAngle -= 360;
    painter.drawArc(boundingRect,
                    startAngle * qtDegPrecision,
                    -static_cast<int>(qtDegPrecision * spanAngle));

    // Draw leading point.
    double radius = boundingRect.width() / 2;
    QPointF point{radius * cos(degreesToRadians(spanAngle - startAngle)),
                  radius * sin(degreesToRadians(spanAngle - startAngle))};

    painter.translate(boundingRect.center());
    painter.drawEllipse(point, pointRadius, pointRadius);
    painter.setBrush(QBrush{Qt::white});
    QPen innerEllipsePen{Qt::white};
    painter.setPen(innerEllipsePen);
    painter.drawEllipse(point, pointInnerRadius, pointInnerRadius);
    painter.resetTransform();
}

void CombinedIndicator::drawText(QPainter& painter) const
{
    constexpr double textSizeRatio{0.2};
    constexpr double penSize{3.0};
    QFont font = painter.font();
    font.setPixelSize(static_cast<int>(textSizeRatio * rect().width()));
    QPen blackPen{Qt::black};
    blackPen.setWidthF(penSize);
    painter.setPen(blackPen);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignCenter, currentText);
}

double degreesToRadians(int degrees)
{
    const double pi{acos(-1)};
    return degrees * pi / 180;
}
