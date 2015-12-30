#include <algorithm>
#include "Pomodoro.h"
#include <QDebug>


Pomodoro::Pomodoro() { }

Pomodoro::Pomodoro(const QString todoName, const QDateTime startTime, const QDateTime finishTime,
                   const QStringList tags, long long associatedTodoItemId) :
        startTime {startTime},
        finishTime {finishTime},
        tags(tags)
{
    todoId = associatedTodoItemId;
    name.append(" ");
    name.append(todoName);
}

Pomodoro::Pomodoro(const TodoItem& todoItem, const QDateTime& startTime, const QDateTime& finishTime) :
        name {todoItem.getName()},
        startTime {startTime},
        finishTime {finishTime},
        tags {todoItem.getTags()}
{
    todoId = todoItem.getId();
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

const QString Pomodoro::toString() const {
    QStringList result;
    QStringList tagsCopy = tags;
    std::for_each(tagsCopy.begin(), tagsCopy.end(), [prefix = tagPrefix](auto& el) {
        return el.prepend(prefix);
    });
    QString start = startTime.time().toString();
    QString finish = finishTime.time().toString();
    start.chop(3);
    finish.chop(3);
    result.append(start);
    result.append(" - ");
    result.append(finish);
    result.append(tagsCopy.join(" "));
    result.append(name);
    return result.join(" ");
}

