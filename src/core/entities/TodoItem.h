#ifndef ENTITIES_H
#define ENTITIES_H

#include "utils/StringUtils.h"
#include <list>
#include <string>
#include <vector>


/* Represent Task that may have many associated Pomodoros.
 *
 * Task has name and tags and might be either in completed or
 * non-completed state. It also has a number of estimated Pomodoros
 * and a number of Pomodoros that were actually spent on this Task. */
class TodoItem {

public:
    TodoItem(std::string name,
             int estimatedPomodoros,
             int spentPomodoros,
             std::list<std::string> tags,
             bool completed);

    /* Construct Task from encoded description.
     * Description is a string of text that may have some words with
     * special prefixes. That prefixes indicate tags and number of
     * estimated Pomodoros for this task.
     *
     * Default values are:
     *      '#' - as a tag prefix;
     *      '*' - as a number of estimated Pomodoros prefix.
     *
     * If description has multiple words with tag prefixes, multiple tags
     * will be assigned to the task, but additional rules apply:
     *
     *      only words with single tag prefix are enterpreted as tags, so
     *      ##Tag will be enterpreted as a part of the name, not as a tag;
     *
     *      single tag prefix (with no characters after it) is also interpreted
     *      as a part of the name.
     *
     * Description can have multiple words with estimated Pomodoros prefixes,
     * but only last of them will be interpreted, previous will be enterpreted
     * as a part of the name. */
    explicit TodoItem(std::string encodedDescription);

    static std::string tagPrefix;

    static std::string estimatedPrefix;

    /* Return Task name. */
    std::string name() const;

    /* Return estimated task cost in pomodoros. */
    int estimatedPomodoros() const;

    /* Pomodoros spent on task. */
    int spentPomodoros() const;

    /* Return list containing Task tags. */
    std::list<std::string> tags() const;

    /* Return string representation of tags.
     * Tags will be concatenated with ', '. */
    std::string tagsAsString() const;

    /* Return true if Task is completed and false otherwise. */
    bool isCompleted() const;

    /* Set task status. */
    void setCompleted(bool completed);

    /* Set number of pomodoros spent on this task. */
    void setSpentPomodoros(int spentPomodoros);

    /* Return string representation of Task. */
    std::string toString() const;

private:
    std::string mName;
    std::string mEncodedDescription;
    int mEstimatedPomodoros = 1;
    int mSpentPomodoros{0};
    std::list<std::string> mTags;
    bool mCompleted{false};

    void decodeDescription(std::string&& encodedDescription);
};

#endif // ENTITIES_H
