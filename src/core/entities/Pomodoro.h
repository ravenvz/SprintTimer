#ifndef POMODORO_POMODORO_H
#define POMODORO_POMODORO_H


#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
#include <QRegularExpression>

class Pomodoro
{

public:
    Pomodoro();
    Pomodoro(const QString& name, const QDateTime& startTime, const QDateTime& finishTime);
    Pomodoro(const QString& todoName, const QDateTime& startTime, const QDateTime& finishTime, const QString& rawTags);
    const QString getName() const;
    void setName(const QString& name);
    const QDateTime getStartTime() const;
    void setStartTime(const QDateTime& startTime);
    const QDateTime getFinishTime() const;
    void setFinishTime(const QDateTime& finishTime);
    const QStringList getTags() const;
    const QString asString() const;

private:
    QString name;
    QDateTime startTime;
    QDateTime finishTime;
    QStringList tags;
    QString tagPrefix = "#";

};



#endif //POMODORO_POMODORO_H
