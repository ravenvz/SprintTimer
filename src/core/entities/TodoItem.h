#ifndef ENTITIES_H
#define ENTITIES_H

#include <QDateTime>
#include <QChar>
#include <QStringList>
#include <QDebug>


class TodoItem
{

public:
    // TODO consider if default c-tor is needed
    TodoItem();

    TodoItem(QString name,
             unsigned estimatedPomodoros,
             unsigned spentPomodoros,
             QStringList tags,
             bool completed);

    TodoItem(QString encodedDescription);

    // Task name.
    QString name() const;

    // Estimation task cost in pomodoros.
    unsigned int estimatedPomodoros() const;

    // Pomodoros spent on task.
    unsigned int spentPomodoros() const;

    // Task tags.
    QStringList tags() const;

    QString tagsAsString() const;

    // Task completion status.
    bool isCompleted() const;

    // Set task status.
    void setCompleted(bool completed);

    // Set number of pomodoros spent on this task.
    void setSpentPomodoros(unsigned int spentPomodoros);

    // Return string representation.
    QString toString() const;

private:
    QString mName;
    unsigned mEstimatedPomodoros = 1;
    unsigned mSpentPomodoros;
    QStringList mTags;
    bool mCompleted;
    QString mEncodedDescription;
    QChar tagPrefix = '#';
    QChar estimatedPrefix = '*';

    QString tagsAsHashedString() const;
    void decodeDescription(QString& encodedDescription);

};

#endif // ENTITIES_H
