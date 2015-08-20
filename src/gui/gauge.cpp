#include <QPainter>
#include "gui/gauge.h"


Gauge::Gauge(QWidget* parent) :
    QWidget(parent)
{
}

void Gauge::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // QRectF size = QRectF(0, 0, this->width() - 65, this->width() - 65);
    QRectF size = QRectF(10, 10, 30, 30);
    painter.setBrush(QColor("#f53d0d"));
    painter.drawPie(size, 0, 90*16);
    painter.setBrush(QColor("#6baa15"));
    painter.drawPie(size, 90*16, 270*16);
}
