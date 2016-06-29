#ifndef POMODORO_POMODORO_H
#define POMODORO_POMODORO_H


#include "core/BoostUUIDGenerator.h"
#include "core/TimeSpan.h"
#include "core/entities/Tag.h"
#include "core/entities/Task.h"
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
             const std::list<Tag>& tags);

    Pomodoro(const std::string& taskName,
             const TimeSpan& timeSpan,
             const std::list<Tag>& tags,
             const std::string& uuid,
             const std::string& taskUuid);

    /* Construct from associated Task and timeSpan. */
    Pomodoro(const Task& task, const TimeSpan& timeSpan);

    Pomodoro(const std::string& taskUuid, const TimeSpan& timeSpan);

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
    std::list<Tag> tags() const;

    // Return string representation.
    std::string toString() const;

private:
    static BoostUUIDGenerator generator;
    std::string mName;
    TimeSpan mInterval;
    std::string mUuid;
    std::string mTaskUuid;
    std::list<Tag> mTags;
};


#endif // POMODORO_POMODORO_H
