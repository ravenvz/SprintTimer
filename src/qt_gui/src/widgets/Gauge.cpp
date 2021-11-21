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

const QColor backgroundFree{QColor{"#354a5f"}};
const QColor backgroundHovered{Qt::white};

} // namespace GaugeColors

namespace {

auto hoveredState = std::make_unique<HoverStateHovered>();
auto unhoveredState = std::make_unique<HoverStateUnhovered>();

} // namespace

Gauge::Gauge(int actual_, int goal_, double gaugeRelSize_, QWidget* parent_)
    : QWidget{parent_}
    , progress{GoalProgress{GoalProgress::Estimated{goal_},
                            GoalProgress::Actual{actual_}}}
    , gaugeRelSize{gaugeRelSize_}
    , hoverState{unhoveredState.get()}
{
    installEventFilter(this);
    setMouseTracking(true);
}

Gauge::Gauge(GoalProgress progress_, double gaugeRelSize_, QWidget* parent_)
    : QWidget{parent_}
    , progress{progress_}
    , gaugeRelSize{gaugeRelSize_}
    , hoverState{unhoveredState.get()}
{
    installEventFilter(this);
    setMouseTracking(true);
}

void Gauge::setData(int completed, int total)
{
    progress = GoalProgress{GoalProgress::Estimated{total},
                            GoalProgress::Actual{completed}};
    repaint();
}

void Gauge::setData(const GoalProgress& newProgress)
{
    progress = newProgress;
    repaint();
}

void Gauge::setData(int indicated,
                    int total,
                    const std::string& emptyColor_,
                    const std::string& filledColor_)
{
    progress = GoalProgress{GoalProgress::Estimated{total},
                            GoalProgress::Actual{indicated}};
    emptyColor = QColor{QString::fromStdString(emptyColor_)};
    filledColor = QColor{QString::fromStdString(filledColor_)};
    repaint();
}

void Gauge::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    setupPainter(painter);

    QRectF totalSizeRect = this->rect();
    double outerRectLength =
        gaugeRelSize * std::min(totalSizeRect.width(), totalSizeRect.height());
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
    painter.setBrush(QBrush(emptyColor));
    painter.drawEllipse(outerRect);
    if (progress.estimated() > 0) {
        // Note that due to integer division angle would be a little off,
        // but for practical purposes this is irrevant,
        // as numerator is big (minimum is 5760) and denominator
        // is estimated to be less then 500 so the difference will
        // negligible (around 0.5 - 1.0 degree).
        const int completedAngle = (progress.actual() % progress.estimated()) *
                                   fullCircle / progress.estimated();
        painter.setBrush(QBrush(filledColor));
        if (completedAngle != 0) {
            painter.drawPie(outerRect, offsetToTop, -completedAngle);
        }
    }
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
    const QPoint toolTipPos{static_cast<int>(event->globalPosition().x()),
                            static_cast<int>(event->globalPosition().y())};
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
    if (gauge.progress.actual() != 0) {
        painter.drawText(gauge.innerRect, Qt::AlignCenter, text);
    }
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

} // namespace sprint_timer::ui::qt_gui
