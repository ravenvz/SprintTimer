#include "core/use_cases/use_cases.h"
#include <algorithm>
#include <vector>
#include <TestHarness.h>

using namespace UseCases;

bool tasksEqual(const TodoItem& lhs, const TodoItem& rhs)
{
    if (lhs.name() != rhs.name()
        || lhs.estimatedPomodoros() != rhs.estimatedPomodoros()
        || lhs.spentPomodoros() != rhs.spentPomodoros()
        || lhs.isCompleted() != rhs.isCompleted()) {
        return false;
    }
    // Tags are compared sorted, because there is no guarantee of tag
    // ordering
    std::list<std::string> tags1 = lhs.tags();
    std::list<std::string> tags2 = rhs.tags();
    tags1.sort();
    tags2.sort();
    return tags1 == tags2;
}


class TestTaskStorageWriter : public ITaskStorageWriter {
public:
    std::vector<TodoItem> storage;

    bool save(const TodoItem& task) final
    {
        storage.push_back(task);
        return true;
    }

    bool remove(const TodoItem& task) final
    {
        auto it
            = std::remove_if(storage.begin(), storage.end(), [&](auto& a_task) {
                  return tasksEqual(task, a_task);
              });
        if (it == storage.end()) {
            return false;
        }
        storage.erase(it, storage.end());
        return true;
    }

    bool edit(const TodoItem& task, const TodoItem& editedTask) final
    {
        auto it = std::find_if(storage.begin(), storage.end(), [&](auto& t) {
            return tasksEqual(task, t);
        });
        if (it == storage.end()) {
            return false;
        }
        it->setCompleted(editedTask.isCompleted());
        it->setName(editedTask.name());
        it->setSpentPomodoros(editedTask.spentPomodoros());
        it->setEstimatedPomodoros(editedTask.estimatedPomodoros());
        it->setTags(editedTask.tags());

        return true;
    }
};


TEST_GROUP(AddTaskUseCase){

};

TEST(AddTaskUseCase, test_name) {}
