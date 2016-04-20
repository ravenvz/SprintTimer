#include "Pomodoro.h"
#include "utils/StringUtils.h"
#include <algorithm>


// Pomodoro::Pomodoro() {}

Pomodoro::Pomodoro(const std::string& taskName,
                   const TimeSpan& timeSpan,
                   const std::list<std::string>& tags)
    : mName{taskName}
    , mInterval{timeSpan}
    , mTags{tags}
{
}

Pomodoro::Pomodoro(const TodoItem& task, const TimeSpan& timeSpan)
    : mName{task.name()}
    , mInterval{timeSpan}
    , mTags{task.tags()}
{
}

std::string Pomodoro::name() const { return mName; }

DateTime Pomodoro::startTime() const { return mInterval.startTime; }

DateTime Pomodoro::finishTime() const { return mInterval.finishTime; }

TimeSpan Pomodoro::timeSpan() const { return mInterval; }

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
