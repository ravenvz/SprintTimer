#ifndef POMODORO_POMODORO_H
#define POMODORO_POMODORO_H


#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
#include <QRegularExpression>
#include "core/timeinterval.h"
#include "TodoItem.h"

class Pomodoro
{

public:
    Pomodoro();
    Pomodoro(const QString todoName, const TimeInterval interval, const QStringList tags);
    Pomodoro(const TodoItem& todoItem, const QDateTime& startTime, const QDateTime& finishTime);

    const QString getName() const;
    const QDateTime getStartTime() const;
    const QDateTime getFinishTime() const;
    const QStringList getTags() const;
    const QString toString() const;

private:
    QString name;
    TimeInterval interval;
    QStringList tags;
    QString tagPrefix = "#";

};



#endif //POMODORO_POMODORO_H
