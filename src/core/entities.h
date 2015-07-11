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

    QString asString() const {
        QStringList result;
        QString start = startTime.time().toString();
        QString finish = finishTime.time().toString();
        start.chop(3);
        finish.chop(3);
        result.append(start);
        result.append(" - ");
        result.append(finish);
        result.append(name);
        return result.join(" ");
    }
};

struct TodoItem {
    TodoItem() {
    }
    TodoItem(QString name,
             unsigned estimatedPomodoros,
             unsigned spentPomodoros,
             int priority,
             QStringList tags,
             bool completed,
             int id) :
        name(name),
        estimatedPomodoros(estimatedPomodoros),
        spentPomodoros(spentPomodoros),
        priority(priority),
        tags(tags),
        completed(completed),
        id(id)
    {
    }
    TodoItem(QString encodedDescription) :
        encodedDescription(encodedDescription)
    {
        decodeDescription(encodedDescription);
        completed = false;
        spentPomodoros = 0;
    }

    QString name;
    unsigned estimatedPomodoros = 0;
    unsigned spentPomodoros;
    int priority;
    QStringList tags;
    bool completed;
    int id;
    QString encodedDescription;

    QString asString() const {
        QStringList result;
        result.append(tagsAsHashedString());
        result.append(name);
        result.append(QString(" %1/%2 ").arg(spentPomodoros).arg(estimatedPomodoros));
        return result.join(" ");
    }

    QString tagsAndNameAsString() const {
        return QString("%1 %2").arg(tagsAsHashedString()).arg(name);
    }

    QString tagsAsString() const {
        QStringList tagsList;
        for (QString tag : tags) {
            tagsList << tag;
        }
        return tagsList.join(" ");
    }

private:
    QStringList nameParts;
    QChar tagPrefix = '#';
    QChar estimatedPrefix = '*';
    QString tagsAsHashedString() const {
        QStringList hashedTags;
        for (QString tag : tags) {
            if (!tag.isEmpty()) {
                hashedTags.append(tag.prepend("#"));
            }
        }
        return hashedTags.join(" ");
    }

    void tryParseTag(QString& part) {
        if (part.startsWith(tagPrefix)) {
            tags << part.right(part.size() - 1);
        }
    }

    void tryParseNamePart(QString& part) {
        if (!part.startsWith(tagPrefix) && !part.startsWith(estimatedPrefix)) {
            nameParts << part;
        }
    }

    void tryParseEstimatedPomodoros(QString& part) {
        if (part.startsWith(estimatedPrefix)) {
            estimatedPomodoros = part.right(part.size() - 1).toInt();
        }
    }

    void decodeDescription(QString& encodedDescription) {
        QStringList parts = encodedDescription.split(" ");
        for (auto part : parts) {
            tryParseTag(part);
            tryParseEstimatedPomodoros(part);
            tryParseNamePart(part);
        }
        name = nameParts.join(' ');
    }
};

#endif // ENTITIES_H
