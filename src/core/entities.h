#ifndef ENTITIES_H
#define ENTITIES_H

#include <QDateTime>
#include <QChar>
#include <QStringList>
#include <QDebug>


class Pomodoro
{

public:
    Pomodoro();
    Pomodoro(const QString& name, QDateTime startTime, QDateTime finishTime);
    const QString& getName() const;
    void setName(const QString& name);
    const QDateTime& getStartTime() const;
    void setStartTime(const QDateTime& startTime);
    const QDateTime& getFinishTime() const;
    void setFinishTime(const QDateTime& finishTime);
    QString asString() const;

private:
    QString name;
    QDateTime startTime;
    QDateTime finishTime;

};

struct TodoItem {
    QString name;
    unsigned estimatedPomodoros = 1;
    unsigned spentPomodoros;
    QStringList tags;
    bool completed;
    int id;
    QString encodedDescription;

    TodoItem(); 
    TodoItem(QString name,
             unsigned estimatedPomodoros,
             unsigned spentPomodoros,
             QStringList tags,
             bool completed,
             int id);
    TodoItem(QString& encodedDescription);
    QString asString() const;
    QString tagsAndNameAsString() const;
    QString tagsAsString() const;

private:
    QChar tagPrefix = '#';
    QChar estimatedPrefix = '*';

    QString tagsAsHashedString() const;
    void decodeDescription(QString& encodedDescription);
};

#endif // ENTITIES_H
