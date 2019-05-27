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
#include "qt_gui/widgets/Gauge.h"
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>
#include <cmath>

namespace {

constexpr int fullCircle{360 * 16};
constexpr int offsetToTop{90 * 16};

} // namespace

namespace sprint_timer::ui::qt_gui {

namespace GaugeColors {
    const QColor normalEmpty{Qt::gray};
    const QColor normalFilled{QColor{"#6baa15"}};
    const QColor overfilledEmpty{QColor{"#6baa15"}};
    const QColor overfilledFilled{Qt::red};
    const QColor backgroundFree{QColor{"#354a5f"}};
    const QColor backgroundHovered{Qt::white};
    const QColor restDayEmpty{QColor{"#ffa500"}};
} // namespace GaugeColors

namespace {
    auto hoveredState = std::make_unique<HoverStateHovered>();
    auto unhoveredState = std::make_unique<HoverStateUnhovered>();
    auto workProgressUnderwork = std::make_unique<WorkProgressUnderwork>();
    auto workProgressOverwork = std::make_unique<WorkProgressOverwork>();
    auto workProgressNone = std::make_unique<WorkProgressNone>();
    auto workProgressDone = std::make_unique<WorkProgressDone>();
    auto workProgressRest = std::make_unique<WorkProgressRest>();
} // namespace

Gauge::Gauge(int actual, int goal, double gaugeRelSize, QWidget* parent)
    : QWidget{parent}
    , progress{GoalProgress{goal, actual}}
    , gaugeRelSize{gaugeRelSize}
    , hoverState{unhoveredState.get()}
{
    installEventFilter(this);
    setMouseTracking(true);

    updateState();
}

Gauge::Gauge(GoalProgress progress, double gaugeRelSize, QWidget* parent)
    : QWidget{parent}
    , progress{progress}
    , gaugeRelSize{gaugeRelSize}
    , hoverState{unhoveredState.get()}
{
    installEventFilter(this);
    setMouseTracking(true);

    updateState();
}

void Gauge::setData(int completed, int total)
{
    progress = GoalProgress{total, completed};
    updateState();
    repaint();
}

void Gauge::setData(const GoalProgress& newProgress)
{
    progress = newProgress;
    updateState();
    repaint();
}

void Gauge::updateState()
{
    if (progress.estimated() == 0) {
        workProgressState = workProgressRest.get();
    }
    else if (progress.actual() == 0) {
        workProgressState = workProgressNone.get();
    }
    else if (progress.actual() == progress.estimated()) {
        workProgressState = workProgressDone.get();
    }
    else if (progress.actual() > progress.estimated()) {
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

void Gauge::mouseMoveEvent(QMouseEvent* event)
{
    const QPoint toolTipPos{event->globalPos().x(), event->globalPos().y()};
    const QString toolTipText = QString{"%1 completed \n%2 estimated"}
                                    .arg(progress.actual())
                                    .arg(progress.estimated());
    setToolTip(toolTipText);
}

void HoverState::drawText(const Gauge& gauge,
                          QPainter& painter,
                          const QString& text)
{
    QFont font = painter.font();
    font.setPixelSize(static_cast<int>(0.3 * gauge.innerRect.width()));
    painter.setFont(font);
    if (gauge.progress.actual() != 0)
        painter.drawText(gauge.innerRect, Qt::AlignCenter, text);
}

void HoverStateHovered::draw(const Gauge& gauge, QPainter& painter)
{
    QString text = QString("%1").arg(gauge.progress.actual());
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);
    painter.drawEllipse(gauge.innerRect);
    HoverState::drawText(gauge, painter, text);
}

void HoverStateUnhovered::draw(const Gauge& gauge, QPainter& painter)
{
    painter.setBrush(GaugeColors::backgroundFree);
    painter.drawEllipse(gauge.innerRect);
    painter.setPen(GaugeColors::backgroundHovered);
    if (auto percentage = gauge.progress.percentage(); percentage)
        HoverState::drawText(
            gauge, painter, QString("%1%").arg(std::floor(*percentage)));
    else
        HoverState::drawText(gauge, painter, "n/a");
}

void WorkProgressState::draw(const Gauge& gauge, QPainter& painter)
{
    setupBrushes();
    int completedAngle = static_cast<int>(
        (gauge.progress.actual() % gauge.progress.estimated()) * fullCircle
        / float(gauge.progress.estimated()));
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

void WorkProgressRest::draw(const Gauge& gauge, QPainter& painter)
{
    //    WorkProgressState::draw(gauge, painter);
    painter.setBrush(GaugeColors::restDayEmpty);
    painter.drawEllipse(gauge.outerRect);
}

void WorkProgressRest::setupBrushes()
{
    empty = QBrush{GaugeColors::restDayEmpty};
}

} // namespace sprint_timer::ui::qt_gui
