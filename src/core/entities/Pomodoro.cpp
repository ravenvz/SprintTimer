#include <algorithm>
#include "Pomodoro.h"
#include <QDebug>


Pomodoro::Pomodoro() {}

Pomodoro::Pomodoro(const QString todoName,
                   const TimeInterval& interval,
                   const QStringList tags)
    : mName{todoName}
    , mInterval{interval}
    , mTags{tags}
{
}

Pomodoro::Pomodoro(const TodoItem& todoItem, const TimeInterval& interval)
    : mName{todoItem.name()}
    , mInterval{interval}
    , mTags{todoItem.tags()}
{
}

QString Pomodoro::name() const { return mName; }

DateTime Pomodoro::startTime() const { return mInterval.startTime; }

DateTime Pomodoro::finishTime() const { return mInterval.finishTime; }

TimeInterval Pomodoro::interval() const { return mInterval; }

QStringList Pomodoro::tags() const { return mTags; }

QString Pomodoro::toString() const
{
    QStringList result;
    QStringList tagsCopy = mTags;
    std::for_each(
        tagsCopy.begin(),
        tagsCopy.end(),
        [prefix = tagPrefix](auto& el) { return el.prepend(prefix); });
    result.append(QString::fromStdString(mInterval.toTimeString()));
    result.append(tagsCopy.join(" "));
    result.append(mName);
    return result.join(" ");
}
