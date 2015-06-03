#ifndef ENTITIES_H
#define ENTITIES_H

#include <chrono>


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

#endif // ENTITIES_H 
