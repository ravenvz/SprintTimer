#include <algorithm>
#include "Pomodoro.h"
#include <QDebug>


Pomodoro::Pomodoro() { }

Pomodoro::Pomodoro(const QString todoName, const TimeInterval interval, const QStringList tags) :
        mName {todoName},
        mInterval {interval},
        mTags {tags}
{

}

Pomodoro::Pomodoro(const TodoItem& todoItem, const QDateTime& startTime, const QDateTime& finishTime) :
        mName {todoItem.name()},
        mInterval {startTime, finishTime},
        mTags {todoItem.tags()}
{

}

QDateTime Pomodoro::finishTime() const {
    return mInterval.finishTime;
}

QDateTime Pomodoro::startTime() const {
    return mInterval.startTime;
}

TimeInterval Pomodoro::interval() const {
    return mInterval;
}

QString Pomodoro::name() const {
    return mName;
}

QStringList Pomodoro::tags() const {
    return mTags;
}

QString Pomodoro::toString() const {
    QStringList result;
    QStringList tagsCopy = mTags;
    std::for_each(tagsCopy.begin(), tagsCopy.end(), [prefix = tagPrefix](auto& el) {
        return el.prepend(prefix);
    });
    result.append(mInterval.toTimeString());
    result.append(tagsCopy.join(" "));
    result.append(mName);
    return result.join(" ");
}

