#include "entities.h"


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


TodoItem::TodoItem(QString encodedDescription) :
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


void TodoItem::tryParseTag(QString& part) {
    if (part.startsWith(tagPrefix)) {
        tags << part.right(part.size() - 1);
    }
}


void TodoItem::tryParseNamePart(QString& part) {
    if (!part.startsWith(tagPrefix) && !part.startsWith(estimatedPrefix)) {
        nameParts << part;
    }
}


void TodoItem::tryParseEstimatedPomodoros(QString& part) {
    if (part.startsWith(estimatedPrefix)) {
        estimatedPomodoros = part.right(part.size() - 1).toInt();
    }
}


void TodoItem::decodeDescription(QString& encodedDescription) {
    QStringList parts = encodedDescription.split(" ");
    for (auto part : parts) {
        tryParseTag(part);
        tryParseEstimatedPomodoros(part);
        tryParseNamePart(part);
    }
    name = nameParts.join(' ');
}
