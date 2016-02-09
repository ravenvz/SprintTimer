#include "TodoItem.h"

#include <QRegularExpression>

TodoItem::TodoItem() {}

TodoItem::TodoItem(QString name,
                   unsigned estimatedPomodoros,
                   unsigned spentPomodoros,
                   QStringList tags,
                   bool completed) :
    mName(name),
    mEstimatedPomodoros(estimatedPomodoros),
    mSpentPomodoros(spentPomodoros),
    mTags(tags),
    mCompleted(completed)
{
}

TodoItem::TodoItem(QString&& encodedDescription) {
    decodeDescription(std::move(encodedDescription));
    mCompleted = false;
    mSpentPomodoros = 0;
}

QString TodoItem::name() const {
    return mName;
}

unsigned int TodoItem::estimatedPomodoros() const {
    return mEstimatedPomodoros;
}

unsigned int TodoItem::spentPomodoros() const {
    return mSpentPomodoros;
}

QStringList TodoItem::tags() const {
    return mTags;
}

bool TodoItem::isCompleted() const {
    return mCompleted;
}

void TodoItem::setCompleted(bool completed) {
    mCompleted = completed;
}

void TodoItem::setSpentPomodoros(unsigned int spentPomodoros) {
    mSpentPomodoros = spentPomodoros;
}

QString TodoItem::toString() const {
    QStringList result;
    result.append(tagsAsHashedString());
    result.append(mName);
    result.append(QString(" %1/%2 ").arg(mSpentPomodoros).arg(mEstimatedPomodoros));
    return result.join(" ");
}

QString TodoItem::tagsAsHashedString() const {
    QStringList hashedTags;
    std::transform(mTags.cbegin(), mTags.cend(), std::back_inserter(hashedTags),
            [](const auto& tag) {
                return QString("#%1").arg(tag);
            });
    return hashedTags.join(" ");
}

void TodoItem::decodeDescription(QString&& encodedDescription) {
    QStringList nameParts;
    QRegularExpression tagRegexp {QString ("^%1\\w+").arg(tagPrefix)};
    encodedDescription.replace(QRegularExpression("\\s+"), " ");
    const QStringList& parts = encodedDescription.split(" ");

    for (const QString& part : parts) {
        if (part.contains(tagRegexp)) {
            mTags << part.right(part.size() - 1);
        } else if (part.startsWith(estimatedPrefix)) {
            mEstimatedPomodoros = part.right(part.size() - 1).toUInt();
        } else {
            nameParts << part;
        }
    }
    mName = nameParts.join(' ');
}
