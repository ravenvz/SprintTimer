#ifndef ENTITIES_H
#define ENTITIES_H

#include <QDateTime>
#include <QChar>
#include <QStringList>
#include <QDebug>


class TodoItem
{

public:
    TodoItem();
    TodoItem(QString name,
             unsigned estimatedPomodoros,
             unsigned spentPomodoros,
             QStringList tags,
             bool completed);
    TodoItem(QString& encodedDescription);
    QString getName() const;
    void setName(const QString& name);
    unsigned int getEstimatedPomodoros() const;
    void setEstimatedPomodoros(unsigned int estimatedPomodoros);
    unsigned int getSpentPomodoros() const;
    void setSpentPomodoros(unsigned int spentPomodoros);
    QStringList getTags() const;
    void setTags(const QStringList& tags);
    bool isCompleted() const;
    void setCompleted(bool completed);
    void toggleCompleted();
    QString getEncodedDescription() const;
    void setEncodedDescription(const QString& encodedDescription);
    QString toString() const;
    QString tagsAndNameAsString() const;
    QString tagsAsString() const;

private:
    QString name;
    unsigned estimatedPomodoros = 1;
    unsigned spentPomodoros;
    QStringList tags;
    bool completed;
    QString encodedDescription;
    QChar tagPrefix = '#';
    QChar estimatedPrefix = '*';

    QString tagsAsHashedString() const;
    void decodeDescription(QString& encodedDescription);

};

#endif // ENTITIES_H
