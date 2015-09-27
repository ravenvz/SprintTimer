#ifndef POMODORO_POMODORO_H
#define POMODORO_POMODORO_H


#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>

class Pomodoro
{

public:
    Pomodoro();
    Pomodoro(const QString& name, QDateTime startTime, QDateTime finishTime);
    const QString& getName() const;
    void setName(const QString& name);
    const QDateTime getStartTime() const;
    void setStartTime(const QDateTime& startTime);
    const QDateTime getFinishTime() const;
    void setFinishTime(const QDateTime& finishTime);
    QString asString() const;

private:
    QString name;
    QDateTime startTime;
    QDateTime finishTime;

};



#endif //POMODORO_POMODORO_H
