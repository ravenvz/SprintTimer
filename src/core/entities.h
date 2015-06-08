#ifndef ENTITIES_H
#define ENTITIES_H

#include <QDateTime>


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
    QString name;
    unsigned estimatedPomodoros;
    unsigned spentPomodoros;
    unsigned priority;
    QStringList tags;
    bool completed;

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

private:
    QString tagsAsHashedString() const {
        QStringList hashedTags;
        for (QString tag : tags) {
            if (!tag.isEmpty()) {
                hashedTags.append(tag.prepend("#"));
            }
        }
        return hashedTags.join(" ");
    }
};

#endif // ENTITIES_H 
