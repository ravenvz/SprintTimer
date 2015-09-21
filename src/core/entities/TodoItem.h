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
             bool completed,
             int id);
    TodoItem(QString& encodedDescription);
    const QString& getName() const;
    void setName(const QString& name);
    unsigned int getEstimatedPomodoros() const;
    void setEstimatedPomodoros(unsigned int estimatedPomodoros);
    unsigned int getSpentPomodoros() const;
    void setSpentPomodoros(unsigned int spentPomodoros);
    const QStringList& getTags() const;
    void setTags(const QStringList& tags);
    bool isCompleted() const;
    void setCompleted(bool completed);
    void toggleCompleted();
    int getId() const;
    void setId(int id);
    const QString& getEncodedDescription() const;
    void setEncodedDescription(const QString& encodedDescription);
    QString asString() const;
    QString tagsAndNameAsString() const;
    QString tagsAsString() const;

private:
    QString name;
    unsigned estimatedPomodoros = 1;
    unsigned spentPomodoros;
    QStringList tags;
    bool completed;
    int id;
    QString encodedDescription;
    QChar tagPrefix = '#';
    QChar estimatedPrefix = '*';

    QString tagsAsHashedString() const;
    void decodeDescription(QString& encodedDescription);

};

#endif // ENTITIES_H
