#include "Pomodoro.h"
#include "utils/StringUtils.h"
#include <algorithm>


Pomodoro::Pomodoro() {}

Pomodoro::Pomodoro(const std::string& taskName,
                   const TimeInterval& interval,
                   const std::list<std::string>& tags)
    : mName{taskName}
    , mInterval{interval}
    , mTags{tags}
{
}

Pomodoro::Pomodoro(const TodoItem& task, const TimeInterval& interval)
    : mName{task.name()}
    , mInterval{interval}
    , mTags{task.tags()}
{
}

std::string Pomodoro::name() const { return mName; }

DateTime Pomodoro::startTime() const { return mInterval.startTime; }

DateTime Pomodoro::finishTime() const { return mInterval.finishTime; }

TimeInterval Pomodoro::interval() const { return mInterval; }

std::list<std::string> Pomodoro::tags() const { return mTags; }

std::string Pomodoro::toString() const
{

    std::vector<std::string> tagsCopy;
    std::vector<std::string> result;

    std::copy(mTags.cbegin(), mTags.cend(), std::back_inserter(tagsCopy));

    std::for_each(tagsCopy.begin(), tagsCopy.end(), [&](auto& elem) {
        elem.insert(0, TodoItem::tagPrefix);
    });

    result.push_back(mInterval.toTimeString());
    result.push_back(StringUtils::join(tagsCopy, std::string(" ")));
    result.push_back(mName);

    return StringUtils::join(result, " ");
}
