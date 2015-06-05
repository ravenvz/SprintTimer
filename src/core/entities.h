#ifndef ENTITIES_H
#define ENTITIES_H

#include <QDateTime>


struct Pomodoro {
    QString name;
    QDateTime startTime;
    QDateTime finishTime;

    QString asString() {
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
};

#endif // ENTITIES_H 
