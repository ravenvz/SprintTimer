#ifndef TASK_H_7VXCYMOK
#define TASK_H_7VXCYMOK


#include "core/BoostUUIDGenerator.h"
#include "core/DateTime.h"
#include "core/StringUtils.h"
#include "core/entities/Tag.h"
#include <list>
#include <string>
#include <vector>


/* Represent Task that may have many associated Pomodoros.
 *
 * Task has name and tags and might be either in completed or
 * non-completed state. It also has a number of estimated Pomodoros
 * and a number of Pomodoros that were actually spent on this Task. */
class Task {

public:
    Task(std::string name,
             int estimatedPomodoros,
             int spentPomodoros,
             std::list<Tag> tags, // TODO should be reference or moved
             bool completed);

    Task(std::string name,
             int estimatedPomodoros,
             int spentPomodoros,
             const std::string& uuid,
             std::list<Tag> tags, // TODO should be reference or moved
             bool completed,
             const DateTime& lastModified);

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
    explicit Task(std::string encodedDescription);

    static std::string estimatedPrefix;

    /* Return Task name. */
    std::string name() const;

    /* Return true if Task is completed and false otherwise. */
    bool isCompleted() const;

    /* Return estimated task cost in pomodoros. */
    int estimatedPomodoros() const;

    /* Number of Pomodoros that had been spent on this task. */
    int spentPomodoros() const;

    /* Return UUID. */
    std::string uuid() const;

    /* Return list containing Task tags. */
    std::list<Tag> tags() const;

    DateTime lastModified() const;

    void setName(const std::string& name);

    /* Set task status. */
    void setCompleted(bool completed);

    void setEstimatedPomodoros(int estimatedPomodoros);

    /* Set number of pomodoros spent on this task. */
    void setSpentPomodoros(int spentPomodoros);

    void setTags(const std::list<Tag>& newTags);

    /* Set time stamp of last item modification. */
    void setModifiedTimeStamp(const DateTime& timeStamp);

    /* Return string representation of tags.
     * Tags will be concatenated with ', '. */
    std::string tagsAsString() const;

    /* Return string representation of Task. */
    std::string toString() const;

private:
    static BoostUUIDGenerator generator;
    std::string mName;
    std::string mEncodedDescription;
    int mEstimatedPomodoros = 1;
    int mSpentPomodoros{0};
    std::string mUuid;
    std::list<Tag> mTags;
    bool mCompleted{false};
    DateTime mLastModified;

    void decodeDescription(std::string&& encodedDescription);
};

#endif /* end of include guard: TASK_H_7VXCYMOK */
