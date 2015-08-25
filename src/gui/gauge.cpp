#include <QPainter>
#include "gui/gauge.h"


Gauge::Gauge(unsigned actual, unsigned goal, QWidget* parent) :
    QWidget(parent),
    actual(actual),
    goal(goal)
{
}

void Gauge::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // QRectF size = QRectF(0, 0, this->width() - 65, this->width() - 65);
    QRectF size = QRectF(10, 10, 30, 30);
    if (goal > 0) {
        int completedAngle = actual * 360 * 16 / goal;
        painter.setBrush(QColor("#6baa15"));
        painter.drawPie(size, 90*16, -completedAngle);
        painter.setBrush(Qt::gray);
        painter.drawPie(size, 90*16, 360*16 - completedAngle);
    } else {
        painter.setBrush(Qt::gray);
        painter.drawPie(size, 90*16, 360*16);
    }
}
