#ifndef POMODORO_POMODORO_H
#define POMODORO_POMODORO_H


#include "TodoItem.h"
#include "core/TimeSpan.h"
#include "utils/UuidUtils.h"
#include <list>
#include <string>

/* Represents Pomodoro concept.
 *
 * Pomodoro is an n-minute time interval that is associated with
 * a Task. It has same name and tags it's associated Task has.
 */
class Pomodoro {

public:
    Pomodoro(const TimeSpan& timeSpan);

    Pomodoro(const std::string& taskName,
             const TimeSpan& timeSpan,
             const std::list<std::string>& tags);

    Pomodoro(const std::string& taskName,
             const TimeSpan& timeSpan,
             const std::list<std::string>& tags,
             const std::string& uuid);

    /* Construct from associated Task and time timeSpan. */
    Pomodoro(const TodoItem& task, const TimeSpan& timeSpan);

    // Return name of Pomodoro. It is identical to the associated task name.
    std::string name() const;

    // Return starting date and time.
    DateTime startTime() const;

    // Return finishing date and time.
    DateTime finishTime() const;

    // Return time interval when this Pomodoro was active.
    TimeSpan timeSpan() const;

    // Return uuid.
    std::string uuid() const;

    // Return associated Task uuid.
    std::string taskUuid() const;

    // Return vector of tags. Tags are identical the associated task tags.
    std::list<std::string> tags() const;

    // Return string representation.
    std::string toString() const;

private:
    static BoostUUUIDGenerator generator;
    std::string mName;
    TimeSpan mInterval;
    std::string mUuid;
    std::string mTaskUuid;
    std::list<std::string> mTags;
};


#endif // POMODORO_POMODORO_H
