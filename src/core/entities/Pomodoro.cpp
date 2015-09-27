#include "Pomodoro.h"


Pomodoro::Pomodoro() { }

Pomodoro::Pomodoro(const QString& name, QDateTime startTime, QDateTime finishTime)
        : name(name), startTime(startTime), finishTime(finishTime) { }

void Pomodoro::setFinishTime(const QDateTime& finishTime) {
    Pomodoro::finishTime = finishTime;
}

const QDateTime Pomodoro::getFinishTime() const {
    return finishTime;
}

void Pomodoro::setStartTime(const QDateTime& startTime) {
    Pomodoro::startTime = startTime;
}

const QDateTime Pomodoro::getStartTime() const {
    return startTime;
}

void Pomodoro::setName(const QString& name) {
    Pomodoro::name = name;
}

const QString& Pomodoro::getName() const {
    return name;
}

QString Pomodoro::asString() const {
    QStringList result;
    QString start = startTime.time().toString();
    QString finish = finishTime.time().toString();
    start.chop(3);
    finish.chop(3);
    result.append(start);
    result.append(" - ");
    result.append(finish);
    result.append(name);
    return result.join(" ");
}

