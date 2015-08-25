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
    int completedAngle;
    if (actual > goal) {
        painter.setBrush(overwork);
        completedAngle = (actual % goal) * fullCircle / goal;
    } else {
        completedAngle = actual * fullCircle / goal;
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
