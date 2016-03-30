#ifndef POMODORO_POMODORO_H
#define POMODORO_POMODORO_H


#include <QtCore/qstring.h>
// #include <QtCore/qdatetime.h>
#include "core/timeinterval.h"
#include "TodoItem.h"

class Pomodoro {

public:
    Pomodoro();

    Pomodoro(const QString todoName,
             const TimeInterval& interval,
             const QStringList tags);

    Pomodoro(const TodoItem& todoItem, const TimeInterval& interval);

    // Return name of Pomodoro. It is identical to the associated task name.
    QString name() const;

    // Return starting date and time.
    DateTime startTime() const;

    // Return finishing date and time.
    DateTime finishTime() const;

    // Return time interval when this Pomodoro was active.
    TimeInterval interval() const;

    // Return tags. Tags are identical the associated task tags.
    QStringList tags() const;

    // Return string representation.
    QString toString() const;

private:
    QString mName;
    TimeInterval mInterval;
    QStringList mTags;
    QString tagPrefix = "#";
};


#endif // POMODORO_POMODORO_H
