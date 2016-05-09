#include "core/use_cases/use_cases.h"
#include <TestHarness.h>
#include <algorithm>
#include <vector>


using namespace UseCases;

Pomodoro createSomePomodoro()
{
    return Pomodoro{
        "Task name placeholder",
        TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()},
        {"Tag1", "Tag2"}};
}

bool pomodoroEqual(const Pomodoro& lhs, const Pomodoro& rhs)
{
    return lhs.startTime() == rhs.startTime();
}

/* Fake implementation of IPomodoroStorageWriter that exposes
 * internal storage and allows to test execute/undo. */
class TestPomodoroGateway : public IPomodoroStorageWriter {
public:
    bool save(const Pomodoro& pomodoro) final
    {
        storage.push_back(pomodoro);
        return true;
    }
    bool remove(const Pomodoro& pomodoro) final
    {
        auto it
            = std::remove_if(storage.begin(), storage.end(), [&](auto& pomo) {
                  return pomodoroEqual(pomo, pomodoro);
              });
        if (it == storage.end()) {
            return false;
        }
        storage.erase(it, storage.end());
        return true;
    }

    std::vector<Pomodoro> storage;
};

class AlwaysSuccessfullGateway : public IPomodoroStorageWriter {
public:
    bool save(const Pomodoro& pomodoro) final { return true; }
    bool remove(const Pomodoro& pomodoro) final { return true; }
};

class AlwaysFailingGateway : public IPomodoroStorageWriter {
public:
    bool save(const Pomodoro& pomodoro) final { return false; }
    bool remove(const Pomodoro& pomodoro) final { return false; }
};

TEST_GROUP(TestAddPomodoroTransaction){

};

TEST(TestAddPomodoroTransaction, test_should_return_true_on_success)
{
    Pomodoro pomodoro = createSomePomodoro();
    AlwaysSuccessfullGateway gateway;
    AddPomodoroTransaction add_pomodoro{gateway, pomodoro};

    CHECK(add_pomodoro.execute());
}

TEST(TestAddPomodoroTransaction, test_should_return_false_when_adding_failed)
{
    Pomodoro pomodoro = createSomePomodoro();
    AlwaysFailingGateway gateway;
    AddPomodoroTransaction add_pomodoro{gateway, pomodoro};

    CHECK(!add_pomodoro.execute());
}

TEST(TestAddPomodoroTransaction,
     test_undo_transaction_that_has_not_been_executed_should_return_false)
{
    Pomodoro pomodoro = createSomePomodoro();
    TestPomodoroGateway gateway;
    AddPomodoroTransaction add_pomodoro{gateway, pomodoro};

    CHECK(!add_pomodoro.undo());
}

TEST(TestAddPomodoroTransaction, test_execute_and_undo)
{
    Pomodoro pomodoro = createSomePomodoro();
    TestPomodoroGateway gateway;
    AddPomodoroTransaction add_pomodoro{gateway, pomodoro};

    CHECK(add_pomodoro.execute());
    CHECK_EQUAL(1, gateway.storage.size());
    CHECK(add_pomodoro.undo());
    CHECK_EQUAL(0, gateway.storage.size());
}

TEST_GROUP(RemovePomodoroTransaction){

};

TEST(RemovePomodoroTransaction,
     test_undo_should_not_add_pomodoro_if_it_was_not_removed_beforehand)
{
    Pomodoro pomodoro = createSomePomodoro();
    TestPomodoroGateway gateway;
    RemovePomodoroTransaction removePomodoro{gateway, pomodoro};

    gateway.storage.push_back(pomodoro);

    CHECK(!removePomodoro.undo());
}

TEST(RemovePomodoroTransaction, test_remove_and_undo)
{
    Pomodoro pomodoro = createSomePomodoro();
    TestPomodoroGateway gateway;
    RemovePomodoroTransaction removePomodoro{gateway, pomodoro};

    gateway.storage.push_back(pomodoro);

    CHECK(removePomodoro.execute());
    CHECK(gateway.storage.empty());
    CHECK(removePomodoro.undo());
    CHECK_EQUAL(1, gateway.storage.size());
}
