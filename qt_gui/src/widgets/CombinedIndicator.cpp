/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#include "widgets/CombinedIndicator.h"
#include <QPainter>

#include <QDebug>

CombinedIndicator::CombinedIndicator(QWidget* parent)
    : QWidget{parent}
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

QSize CombinedIndicator::sizeHint() const { return QSize(150, 150); }

void CombinedIndicator::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawProgress(painter);
}

void CombinedIndicator::drawProgress(QPainter& painter) const
{
    if (timerDuration == 0)
        return;
    if (timeLeft != 0)
        drawProgressBar(painter);
    drawStatusText(painter);
}

void CombinedIndicator::drawProgressBar(QPainter& painter) const
{
    int margin = static_cast<int>(this->rect().width() * 0.05);
    QRectF boundingRect
        = this->rect().adjusted(margin, margin, -margin, -margin);
    QPen progressPen{color};
    progressPen.setWidthF(4.0);
    painter.setPen(progressPen);
    painter.drawRect(this->rect());
    int startAngle = 90 * 16;
    int spanAngle = (timeLeft - timerDuration) * 16 * (360 / timerDuration);
    painter.drawArc(boundingRect, startAngle, spanAngle);
}

void CombinedIndicator::drawStatusText(QPainter& painter) const
{
    QFont font = painter.font();
    font.setPixelSize(static_cast<int>(0.2 * this->rect().width()));
    QPen blackPen{Qt::black};
    blackPen.setWidthF(3.0);
    painter.setPen(blackPen);
    painter.setFont(font);
    painter.drawText(this->rect(), Qt::AlignCenter, status);
}

void CombinedIndicator::mousePressEvent(QMouseEvent* event)
{
    qDebug() << event;
    if (event->button() == Qt::LeftButton) {
        emit indicatorClicked();
    }
}

void CombinedIndicator::updateIndication(Second timeLeft)
{
    this->timeLeft = timeLeft;
}

void CombinedIndicator::setDuration(Second duration)
{
    timerDuration = duration;
}

void CombinedIndicator::setStatus(QString&& status)
{
    this->status = std::move(status);
}

void CombinedIndicator::setColor(const QColor& color) { this->color = color; }
