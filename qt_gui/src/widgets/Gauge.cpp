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
#include "Gauge.h"
#include <QPainter>

namespace sprint_timer::ui::qt_gui {


namespace GaugeColors {
const QColor normalEmpty{Qt::gray};
const QColor normalFilled{QColor{"#6baa15"}};
const QColor overfilledEmpty{QColor{"#6baa15"}};
const QColor overfilledFilled{Qt::red};
const QColor backgroundFree{QColor{"#354a5f"}};
const QColor backgroundHovered{Qt::white};
};

namespace {
auto hoveredState{std::make_unique<HoverStateHovered>()};
auto unhoveredState{std::make_unique<HoverStateUnhovered>()};
auto workProgressUnderwork{std::make_unique<WorkProgressUnderwork>()};
auto workProgressOverwork{std::make_unique<WorkProgressOverwork>()};
auto workProgressNone{std::make_unique<WorkProgressNone>()};
auto workProgressDone{std::make_unique<WorkProgressDone>()};
}

Gauge::Gauge(int actual, int goal, double gaugeRelSize, QWidget* parent)
    : QWidget(parent)
    , actual{actual}
    , goal{goal}
    , gaugeRelSize{gaugeRelSize}
    , hoverState{unhoveredState.get()}
{
    installEventFilter(this);
    setMouseTracking(true);

    updateState();
}

void Gauge::setData(int completed, int total)
{
    actual = completed;
    goal = total;
    updateState();
    repaint();
}

void Gauge::updateState()
{
    if (actual == 0 || goal == 0) {
        workProgressState = workProgressNone.get();
    }
    else if (actual == goal) {
        workProgressState = workProgressDone.get();
    }
    else if (actual > goal) {
        workProgressState = workProgressOverwork.get();
    }
    else {
        workProgressState = workProgressUnderwork.get();
    }
}

void Gauge::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    setupPainter(painter);

    QRectF totalSizeRect = this->rect();
    double outerRectLength = gaugeRelSize
        * std::min(totalSizeRect.width(), totalSizeRect.height());
    double innerRectLength = 0.7 * outerRectLength;
    outerRect = QRectF(totalSizeRect.center().x() - outerRectLength / 2,
                       totalSizeRect.center().y() - outerRectLength / 2,
                       outerRectLength,
                       outerRectLength);
    innerRect = QRectF(totalSizeRect.center().x() - innerRectLength / 2,
                       totalSizeRect.center().y() - innerRectLength / 2,
                       innerRectLength,
                       innerRectLength);

    drawOuterCircle(painter);
    drawInnerCircle(painter);
}

void Gauge::setupPainter(QPainter& painter)
{
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidthF(1.2);
    painter.setPen(pen);
}

void Gauge::drawOuterCircle(QPainter& painter)
{
    workProgressState->draw(*this, painter);
}

void Gauge::drawInnerCircle(QPainter& painter)
{
    hoverState->draw(*this, painter);
}

bool Gauge::eventFilter(QObject* object, QEvent* event)
{
    if (object != this)
        return false;
    if (event->type() == QEvent::Enter) {
        setFocus();
        hoverState = hoveredState.get();
        this->repaint();
        return true;
    }
    if (event->type() == QEvent::Leave) {
        hoverState = unhoveredState.get();
        this->repaint();
        return true;
    }
    return false;
}

void HoverState::drawText(const Gauge& gauge,
                          QPainter& painter,
                          const QString& text)
{
    QFont font = painter.font();
    font.setPixelSize(static_cast<int>(0.3 * gauge.innerRect.width()));
    painter.setFont(font);
	if (gauge.actual != 0)
		painter.drawText(gauge.innerRect, Qt::AlignCenter, text);
}

void HoverStateHovered::draw(const Gauge& gauge, QPainter& painter)
{
    QString text = QString("%1").arg(gauge.actual);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);
    painter.drawEllipse(gauge.innerRect);
    HoverState::drawText(gauge, painter, text);
}

void HoverStateUnhovered::draw(const Gauge& gauge, QPainter& painter)
{
    QString text = QString("%1%").arg(
        gauge.goal != 0 ? gauge.actual * 100 / gauge.goal : 0);
    painter.setBrush(GaugeColors::backgroundFree);
    painter.drawEllipse(gauge.innerRect);
    painter.setPen(GaugeColors::backgroundHovered);
    HoverState::drawText(gauge, painter, text);
}

void WorkProgressState::draw(const Gauge& gauge, QPainter& painter)
{
    setupBrushes();
    constexpr int fullCircle = 360 * 16;
    constexpr int offsetToTop = 90 * 16;
    int completedAngle = static_cast<int>(
        (gauge.actual % gauge.goal) * fullCircle / float(gauge.goal));
    painter.setBrush(empty);
    painter.drawEllipse(gauge.outerRect);
    painter.setBrush(filled);
    painter.drawPie(gauge.outerRect, offsetToTop, -completedAngle);
}

void WorkProgressState::setupBrushes() {}

void WorkProgressUnderwork::setupBrushes()
{
    empty = QBrush{GaugeColors::normalEmpty};
    filled = QBrush{GaugeColors::normalFilled};
}

void WorkProgressOverwork::setupBrushes()
{
    empty = QBrush{GaugeColors::overfilledEmpty};
    filled = QBrush{GaugeColors::overfilledFilled};
}

void WorkProgressDone::draw(const Gauge& gauge, QPainter& painter)
{
    painter.setBrush(GaugeColors::normalFilled);
    painter.drawEllipse(gauge.outerRect);
}

void WorkProgressNone::draw(const Gauge& gauge, QPainter& painter)
{
    painter.setBrush(GaugeColors::normalEmpty);
    painter.drawEllipse(gauge.outerRect);
}

} // namespace sprint_timer::ui::qt_gui

