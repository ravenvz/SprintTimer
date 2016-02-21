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
             int estimatedPomodoros,
             int spentPomodoros,
             QStringList tags,
             bool completed);

    explicit TodoItem(QString encodedDescription);

    // Task name.
    QString name() const;

    // Estimation task cost in pomodoros.
    int estimatedPomodoros() const;

    // Pomodoros spent on task.
    int spentPomodoros() const;

    // Task tags.
    QStringList tags() const;

    QString tagsAsString() const;

    // Task completion status.
    bool isCompleted() const;

    // Set task status.
    void setCompleted(bool completed);

    // Set number of pomodoros spent on this task.
    void setSpentPomodoros(int spentPomodoros);

    // Return string representation.
    QString toString() const;

private:
    QString mName;
    int mEstimatedPomodoros = 1;
    int mSpentPomodoros;
    QStringList mTags;
    bool mCompleted;
    QString mEncodedDescription;
    QChar tagPrefix = '#';
    QChar estimatedPrefix = '*';

    QString tagsAsHashedString() const;
    void decodeDescription(QString& encodedDescription);

};

#endif // ENTITIES_H
