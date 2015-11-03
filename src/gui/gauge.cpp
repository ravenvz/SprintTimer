#include <QPainter>
#include "gui/gauge.h"


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
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidthF(1.2);
    painter.setPen(pen);
    if (displayDetails) {
       painter.drawEllipse(size);
        painter.drawText(size, Qt::AlignCenter, QString("%1").arg(actual));
    } else {
        if (goal == 0 || actual == 0) {
            painter.setBrush(noWork);
            painter.drawEllipse(size);
            return;
        }
        if (goal == actual) {
            painter.setBrush(normalWork);
            painter.drawEllipse(size);
            return;
        }
        float completedAngle;
        if (actual > goal) {
            painter.setBrush(overwork);
            completedAngle = (actual % goal) * fullCircle / float(goal);
        } else {
            completedAngle = actual * fullCircle / float(goal);
            painter.setBrush(normalWork);
        }
        painter.drawPie(size, offsetToTop, -completedAngle);
        if (actual > goal) {
            painter.setBrush(normalWork);
        } else {
            painter.setBrush(noWork);
        }
        painter.drawPie(size, offsetToTop, fullCircle - completedAngle);
    }
}

bool Gauge::eventFilter(QObject* object, QEvent* event) {
    if (object == this && (event->type() == QEvent::Enter || event->type() == QEvent::Leave)) {
        displayDetails = event->type() == QEvent::Enter;
        this->repaint();
        return true;
    }
    return false;
}
