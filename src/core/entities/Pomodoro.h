#ifndef POMODORO_POMODORO_H
#define POMODORO_POMODORO_H


#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
#include "core/timeinterval.h"
#include "TodoItem.h"

class Pomodoro
{

public:
    Pomodoro();
    Pomodoro(const QString todoName, const TimeInterval interval, const QStringList tags);
    Pomodoro(const TodoItem& todoItem, const QDateTime& startTime, const QDateTime& finishTime);

    QString name() const;
    QDateTime startTime() const;
    QDateTime finishTime() const;
    TimeInterval interval() const;
    QStringList tags() const;
    QString toString() const;

private:
    QString mName;
    TimeInterval mInterval;
    QStringList mTags;
    QString tagPrefix = "#";

};



#endif //POMODORO_POMODORO_H
