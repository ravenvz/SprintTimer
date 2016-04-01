#ifndef POMODORO_POMODORO_H
#define POMODORO_POMODORO_H


#include <QtCore/qstring.h>
#include <string>
#include "core/timeinterval.h"
#include "TodoItem.h"

class Pomodoro {

public:
    Pomodoro();

    Pomodoro(const std::string& todoName,
             const TimeInterval& interval,
             const QStringList& tags);

    Pomodoro(const TodoItem& todoItem, const TimeInterval& interval);

    // Return name of Pomodoro. It is identical to the associated task name.
    std::string name() const;

    // Return starting date and time.
    DateTime startTime() const;

    // Return finishing date and time.
    DateTime finishTime() const;

    // Return time interval when this Pomodoro was active.
    TimeInterval interval() const;

    // Return vector of tags. Tags are identical the associated task tags.
    std::vector<std::string> tags() const;

    // Return string representation.
    std::string toString() const;

private:
    std::string mName;
    TimeInterval mInterval;
    std::vector<std::string> mTags;
    std::string tagPrefix{"#"};
};


#endif // POMODORO_POMODORO_H
