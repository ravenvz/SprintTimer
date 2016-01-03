#include <algorithm>
#include "Pomodoro.h"
#include <QDebug>


Pomodoro::Pomodoro() { }

Pomodoro::Pomodoro(const QString todoName, const TimeInterval interval, const QStringList tags,
                   long long associatedTodoItemId) :
        interval {interval},
        tags(tags)
{
    todoId = associatedTodoItemId;
    name.append(" ");
    name.append(todoName);
}

Pomodoro::Pomodoro(const TodoItem& todoItem, const QDateTime& startTime, const QDateTime& finishTime) :
        name {todoItem.getName()},
        interval {startTime, finishTime},
        tags {todoItem.getTags()}
{
    todoId = todoItem.getId();
}

void Pomodoro::setFinishTime(const QDateTime finishTime) {
    interval.finishTime = finishTime;
}

const QDateTime Pomodoro::getFinishTime() const {
    return interval.finishTime;
}

void Pomodoro::setStartTime(const QDateTime startTime) {
    interval.startTime = startTime;
}

const QDateTime Pomodoro::getStartTime() const {
    return interval.startTime;
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

const QString Pomodoro::toString() const {
    QStringList result;
    QStringList tagsCopy = tags;
    std::for_each(tagsCopy.begin(), tagsCopy.end(), [prefix = tagPrefix](auto& el) {
        return el.prepend(prefix);
    });
    result.append(interval.toTimeString());
    result.append(tagsCopy.join(" "));
    result.append(name);
    return result.join(" ");
}

