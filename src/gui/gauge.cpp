#include <QPainter>
#include <QWidget>
#include "gui/gauge.h"

#include <QDebug>


Gauge::Gauge(unsigned actual, unsigned goal, QWidget* parent) :
    QWidget(parent),
    actual(actual),
    goal(goal)
{
    installEventFilter(this);
    setMouseTracking(true);
}

void Gauge::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    setupPainter(painter);
    drawOuterCircle(painter);
    drawInnerCircle(painter);
}

void Gauge::setupPainter(QPainter& painter) {
    if (!sizesComputed)
        computeAdaptiveSizes();
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidthF(1.2);
    painter.setPen(pen);
}

void Gauge::computeAdaptiveSizes() {
    QRectF totalSizeRect = this->rect();
    double outerRectLength = 0.8 * std::min(totalSizeRect.width(), totalSizeRect.height());
    double innerRectLength = 0.7 * outerRectLength;
    outerRect = QRectF(totalSizeRect.center().x() - outerRectLength / 2,
                       totalSizeRect.center().y() - outerRectLength / 2,
                       outerRectLength,
                       outerRectLength);
    innerRect = QRectF(totalSizeRect.center().x() - innerRectLength / 2,
                       totalSizeRect.center().y() - innerRectLength / 2,
                       innerRectLength,
                       innerRectLength);
    sizesComputed = true;
}

void Gauge::drawOuterCircle(QPainter& painter) {
    const int fullCircle = 360 * 16;
    float completedAngle = (actual % goal) * fullCircle / float(goal);
    painter.setBrush(empty);
    painter.drawEllipse(outerRect);
    painter.setBrush(filled);
    painter.drawPie(outerRect, offsetToTop, -completedAngle);
}

void Gauge::drawInnerCircle(QPainter& painter) {
    QString text;
    if (displayDetails) {
        text = QString("%1").arg(actual);
        painter.setBrush(Qt::white);
        painter.setPen(Qt::black);
        painter.drawEllipse(innerRect);
    } else {
        text = QString("%1%").arg(actual*100/goal);
        painter.setBrush(QColor("#354a5f"));
        painter.drawEllipse(innerRect);
        painter.setPen(Qt::white);
    }
    QFont font = painter.font();
    font.setPixelSize(0.3 * innerRect.width());
    painter.setFont(font);
    painter.drawText(innerRect, Qt::AlignCenter, text);
}

void EmptyGauge::drawOuterCircle(QPainter& painter) {
    painter.setBrush(empty);
    painter.drawEllipse(outerRect);
}

void FilledGauge::drawOuterCircle(QPainter& painter) {
    painter.setBrush(filled);
    painter.drawEllipse(outerRect);
}

// void EmptyGauge::drawInnerCircle(QPainter& painter) {
//     if (displayDetails) {
//         painter.setBrush(Qt::white);
//         drawInnerCircleHint(painter);
//     }
//     painter.drawEllipse(innerRect);
// }

bool Gauge::eventFilter(QObject* object, QEvent* event) {
    if (object == this && (event->type() == QEvent::Enter || event->type() == QEvent::Leave)) {
        displayDetails = event->type() == QEvent::Enter;
        this->repaint();
        return true;
    }
    return false;
}

EmptyGauge::EmptyGauge(unsigned actual, unsigned goal, QWidget* parent) :
    Gauge(actual, goal, parent)
{
}

FilledGauge::FilledGauge(unsigned actual, unsigned goal, QWidget* parent) :
    Gauge(actual, goal, parent)
{
}

OverfilledGauge::OverfilledGauge(unsigned actual, unsigned goal, QWidget* parent) :
    Gauge(actual, goal, parent)
{
    filled = Qt::red;
    empty = QColor("#6baa15");
}

