#include "entities.h"

#include <QRegularExpression>

Pomodoro::Pomodoro() { }

Pomodoro::Pomodoro(const QString& name, QDateTime startTime, QDateTime finishTime)
        : name(name), startTime(startTime), finishTime(finishTime) { }

void Pomodoro::setFinishTime(const QDateTime& finishTime) {
    Pomodoro::finishTime = finishTime;
}

const QDateTime& Pomodoro::getFinishTime() const {
    return finishTime;
}

void Pomodoro::setStartTime(const QDateTime& startTime) {
    Pomodoro::startTime = startTime;
}

const QDateTime& Pomodoro::getStartTime() const {
    return startTime;
}

void Pomodoro::setName(const QString& name) {
    Pomodoro::name = name;
}

const QString& Pomodoro::getName() const {
    return name;
}

QString Pomodoro::asString() const {
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


TodoItem::TodoItem() {
    
}


TodoItem::TodoItem(QString name,
         unsigned estimatedPomodoros,
         unsigned spentPomodoros,
         QStringList tags,
         bool completed,
         int id) :
    name(name),
    estimatedPomodoros(estimatedPomodoros),
    spentPomodoros(spentPomodoros),
    tags(tags),
    completed(completed),
    id(id)
{
}


TodoItem::TodoItem(QString& encodedDescription) :
    encodedDescription(encodedDescription)
{
    decodeDescription(encodedDescription);
    completed = false;
    spentPomodoros = 0;
}


QString TodoItem::asString() const {
    QStringList result;
    result.append(tagsAsHashedString());
    result.append(name);
    result.append(QString(" %1/%2 ").arg(spentPomodoros).arg(estimatedPomodoros));
    return result.join(" ");
}


QString TodoItem::tagsAndNameAsString() const {
    return QString("%1 %2").arg(tagsAsHashedString()).arg(name);
}


QString TodoItem::tagsAsString() const {
    QStringList tagsList;
    for (QString tag : tags) {
        tagsList << tag;
    }
    return tagsList.join(" ");
}


QString TodoItem::tagsAsHashedString() const {
    QStringList hashedTags;
    for (QString tag : tags) {
        if (!tag.isEmpty()) {
            hashedTags.append(tag.prepend("#"));
        }
    }
    return hashedTags.join(" ");
}


void TodoItem::decodeDescription(QString& encodedDescription) {
    QStringList nameParts;
    QRegularExpression tagRegexp {QString ("^%1\\w+").arg(tagPrefix)};
    encodedDescription.replace(QRegularExpression("\\s+"), " ");
    QStringList parts = encodedDescription.split(" ");
    for (QString part : parts) {
        if (part.contains(tagRegexp)) {
            tags << part.right(part.size() - 1);
        } else if (part.startsWith(estimatedPrefix)) {
            estimatedPomodoros = part.right(part.size() - 1).toUInt();
        } else {
            nameParts << part;
        }
    }
    name = nameParts.join(' ');
}

