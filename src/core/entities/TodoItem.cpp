#include "TodoItem.h"

#include <QRegularExpression>

TodoItem::TodoItem() {}

TodoItem::TodoItem(QString name,
                   unsigned estimatedPomodoros,
                   unsigned spentPomodoros,
                   QStringList tags,
                   bool completed,
                   long long id) :
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

void TodoItem::setEncodedDescription(const QString& encodedDescription) {
    TodoItem::encodedDescription = encodedDescription;
}

QString TodoItem::getEncodedDescription() const {
    return encodedDescription;
}

void TodoItem::setId(long long id) {
    TodoItem::id = id;
}

long long TodoItem::getId() const {
    return id;
}

void TodoItem::setCompleted(bool completed) {
    TodoItem::completed = completed;
}

bool TodoItem::isCompleted() const {
    return completed;
}

void TodoItem::toggleCompleted() {
    TodoItem::completed = !TodoItem::completed;
}

void TodoItem::setTags(const QStringList& tags) {
    TodoItem::tags = tags;
}

QStringList TodoItem::getTags() const {
    return tags;
}

void TodoItem::setSpentPomodoros(unsigned int spentPomodoros) {
    TodoItem::spentPomodoros = spentPomodoros;
}

unsigned int TodoItem::getSpentPomodoros() const {
    return spentPomodoros;
}

void TodoItem::setEstimatedPomodoros(unsigned int estimatedPomodoros) {
    TodoItem::estimatedPomodoros = estimatedPomodoros;
}

unsigned int TodoItem::getEstimatedPomodoros() const {
    return estimatedPomodoros;
}

void TodoItem::setName(const QString& name) {
    TodoItem::name = name;
}

QString TodoItem::getName() const {
    return name;
}

QString TodoItem::toString() const {
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
