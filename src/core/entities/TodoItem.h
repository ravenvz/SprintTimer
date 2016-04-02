#ifndef ENTITIES_H
#define ENTITIES_H

#include <string>
#include <vector>
#include <list>
#include "utils/StringUtils.h"


class TodoItem {

public:
    // TODO consider if default c-tor is needed
    TodoItem();

    TodoItem(std::string name,
             int estimatedPomodoros,
             int spentPomodoros,
             std::list<std::string> tags,
             bool completed);

    explicit TodoItem(std::string encodedDescription);

    // Task name.
    std::string name() const;

    // Estimation task cost in pomodoros.
    int estimatedPomodoros() const;

    // Pomodoros spent on task.
    int spentPomodoros() const;

    // Task tags.
    std::list<std::string> tags() const;

    std::string tagsAsString() const;

    // Task completion status.
    bool isCompleted() const;

    // Set task status.
    void setCompleted(bool completed);

    // Set number of pomodoros spent on this task.
    void setSpentPomodoros(int spentPomodoros);

    // Return string representation.
    std::string toString() const;

private:
    std::string mName;
    std::string mEncodedDescription;
    int mEstimatedPomodoros = 1;
    int mSpentPomodoros;
    std::list<std::string> mTags;
    bool mCompleted;
    std::string tagPrefix{"#"};
    std::string estimatedPrefix{"*"};

    // std::string tagsAsHashedString() const;
    void decodeDescription(std::string&& encodedDescription);
};

#endif // ENTITIES_H
