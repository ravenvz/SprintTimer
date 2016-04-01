#include <algorithm>
#include "Pomodoro.h"
#include <QDebug>
#include "utils/StringUtils.h"


Pomodoro::Pomodoro() {}

Pomodoro::Pomodoro(const std::string& todoName,
                   const TimeInterval& interval,
                   const QStringList& tags)
    : mName{todoName}
    , mInterval{interval}
{
    std::transform(tags.cbegin(),
                   tags.cend(),
                   std::back_inserter(mTags),
                   [](const auto& elem) { return elem.toStdString(); });
}

Pomodoro::Pomodoro(const TodoItem& todoItem, const TimeInterval& interval)
    : mName{todoItem.name().toStdString()}
    , mInterval{interval}
{
    auto tags = todoItem.tags();
    std::transform(tags.cbegin(),
                   tags.cend(),
                   std::back_inserter(mTags),
                   [](const auto& elem) { return elem.toStdString(); });
}

std::string Pomodoro::name() const { return mName; }

DateTime Pomodoro::startTime() const { return mInterval.startTime; }

DateTime Pomodoro::finishTime() const { return mInterval.finishTime; }

TimeInterval Pomodoro::interval() const { return mInterval; }

std::vector<std::string> Pomodoro::tags() const { return mTags; }

std::string Pomodoro::toString() const
{

    QStringList result;
    std::vector<std::string> tagsCopy;
    std::vector<std::string> res;

    std::copy(mTags.cbegin(), mTags.cend(), std::back_inserter(tagsCopy));

    std::for_each(tagsCopy.begin(),
                  tagsCopy.end(),
                  [&](auto& elem) { elem.insert(0, tagPrefix); });

    res.push_back(mInterval.toTimeString());
    res.push_back(join(tagsCopy, std::string(" ")));
    res.push_back(mName);

    return join(res, " ");
}
