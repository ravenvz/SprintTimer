#include <algorithm>
#include "Pomodoro.h"
#include <QDebug>


Pomodoro::Pomodoro() { }

Pomodoro::Pomodoro(const QString& name, const QDateTime& startTime, const QDateTime& finishTime)
        : name(name), startTime(startTime), finishTime(finishTime) 
{
    QRegularExpression tagRegexp {QString ("(%1\\S+)").arg(tagPrefix)};
    QRegularExpressionMatchIterator it = tagRegexp.globalMatch(name);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        tags << match.captured(1);
    }
}

Pomodoro::Pomodoro(const QString& todoName, const QDateTime& startTime, const QDateTime& finishTime, const QString& rawTags) :
        startTime(startTime), finishTime(finishTime) 
{
    if (!rawTags.size() == 0) {
        QStringList tagList = rawTags.split(",");
        std::for_each(tagList.begin(), tagList.end(), [tagPrefix = tagPrefix](auto& el) {
                return el.prepend(tagPrefix); 
            });
        name.append(tagList.join(" "));
    }
    name.append(" ");
    name.append(todoName);
}

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

const QString Pomodoro::getName() const {
    return name;
}

const QStringList Pomodoro::getTags() const {
    return tags;
}

const QString Pomodoro::asString() const {
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

