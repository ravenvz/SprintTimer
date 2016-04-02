#include <algorithm>
#include "Pomodoro.h"
#include <QDebug>
#include "utils/StringUtils.h"


Pomodoro::Pomodoro() {}

Pomodoro::Pomodoro(const std::string& todoName,
                   const TimeInterval& interval,
                   const std::list<std::string>& tags)
    : mName{todoName}
    , mInterval{interval}
    , mTags{tags}
{
}

Pomodoro::Pomodoro(const TodoItem& todoItem, const TimeInterval& interval)
    : mName{todoItem.name()}
    , mInterval{interval}
    , mTags{todoItem.tags()}
{
}

std::string Pomodoro::name() const { return mName; }

DateTime Pomodoro::startTime() const { return mInterval.startTime; }

DateTime Pomodoro::finishTime() const { return mInterval.finishTime; }

TimeInterval Pomodoro::interval() const { return mInterval; }

std::list<std::string> Pomodoro::tags() const { return mTags; }

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
    res.push_back(StringUtils::join(tagsCopy, std::string(" ")));
    res.push_back(mName);

    return StringUtils::join(res, " ");
}
