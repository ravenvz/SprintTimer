#include "Pomodoro.h"
#include "utils/StringUtils.h"
#include <algorithm>


// static
BoostUUUIDGenerator Pomodoro::generator;


//TODO too much constructors that instantiate partial pomodoros.

Pomodoro::Pomodoro(const TimeSpan& timeSpan)
    : mInterval{timeSpan}
    , mUuid{generator.generateUUID()}
{
}

Pomodoro::Pomodoro(const std::string& taskName,
                   const TimeSpan& timeSpan,
                   const std::list<std::string>& tags)
    : mName{taskName}
    , mInterval{timeSpan}
    , mUuid{generator.generateUUID()}
    , mTags{tags}
{
}

Pomodoro::Pomodoro(const std::string& taskName,
                   const TimeSpan& timeSpan,
                   const std::list<std::string>& tags,
                   const std::string& uuid,
                   const std::string& taskUuid)
    : mName{taskName}
    , mInterval{timeSpan}
    , mUuid{uuid}
    , mTaskUuid{taskUuid}
    , mTags{tags}
{
}

Pomodoro::Pomodoro(const TodoItem& task, const TimeSpan& timeSpan)
    : mName{task.name()}
    , mInterval{timeSpan}
    , mUuid{generator.generateUUID()}
    , mTaskUuid{task.uuid()}
    , mTags{task.tags()}
{
}

std::string Pomodoro::name() const { return mName; }

DateTime Pomodoro::startTime() const { return mInterval.startTime; }

DateTime Pomodoro::finishTime() const { return mInterval.finishTime; }

TimeSpan Pomodoro::timeSpan() const { return mInterval; }

std::string Pomodoro::uuid() const { return mUuid; }

std::string Pomodoro::taskUuid() const { return mTaskUuid; }

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
