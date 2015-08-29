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
    painter.setRenderHint(QPainter::Antialiasing);
    if (goal == 0 || actual == 0) {
        painter.setBrush(noWork);
        painter.drawEllipse(gaugeSize);
        return;
    }
    if (goal == actual) {
        painter.setBrush(normalWork);
        painter.drawEllipse(gaugeSize);
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
    painter.drawPie(gaugeSize, offsetToTop, -completedAngle);
    if (actual > goal) {
        painter.setBrush(normalWork);
    } else {
        painter.setBrush(noWork);
    }
    painter.drawPie(gaugeSize, offsetToTop, fullCircle - completedAngle);
}
