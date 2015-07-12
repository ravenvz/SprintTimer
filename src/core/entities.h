#ifndef ENTITIES_H
#define ENTITIES_H

#include <QDateTime>
#include <QChar>
#include <QStringList>
#include <QDebug>


struct Pomodoro {
    QString name;
    QDateTime startTime;
    QDateTime finishTime;

    QString asString() const;
};

struct TodoItem {
    QString name;
    unsigned estimatedPomodoros = 0;
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
    TodoItem(QString encodedDescription);
    QString asString() const;
    QString tagsAndNameAsString() const;
    QString tagsAsString() const;

private:
    QStringList nameParts;
    QChar tagPrefix = '#';
    QChar estimatedPrefix = '*';

    QString tagsAsHashedString() const;
    void tryParseTag(QString& part);
    void tryParseNamePart(QString& part);
    void tryParseEstimatedPomodoros(QString& part);
    void decodeDescription(QString& encodedDescription);
};

#endif // ENTITIES_H
