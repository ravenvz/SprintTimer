#include "models/pomodoromodel.h"
#include "models/todoitemmodel.h"
#include <TestHarness.h>
#include <chrono>

#include <QSqlRecord>


TEST_GROUP(PomodoroModel){

    void setup(){
        // Memory leak checker does not play well with Qt objects at all.
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
QSqlQuery query;
query.exec("delete from pomodoro;");
query.exec("delete from todo_item");
query.exec("delete from todotag");
query.exec("delete from tag");
}

void teardown() { MemoryLeakWarningPlugin::turnOnNewDeleteOverloads(); }

bool timeSpan_equals_to_minute(const TimeSpan& lhs, const TimeSpan& rhs)
{
    return (lhs.startTime.year() == rhs.startTime.year())
        && (lhs.startTime.hour() == rhs.startTime.hour())
        && (lhs.startTime.month() == rhs.startTime.month())
        && (lhs.startTime.minute() == rhs.startTime.minute())
        && (lhs.finishTime.year() == rhs.finishTime.year())
        && (lhs.finishTime.hour() == rhs.finishTime.hour())
        && (lhs.finishTime.month() == rhs.finishTime.month())
        && (lhs.finishTime.minute() == rhs.finishTime.minute());
}

bool pomodoro_equal(const Pomodoro& pomodoro1, const Pomodoro& pomodoro2)
{
    if (pomodoro1.name() != pomodoro2.name()) {
        std::cout << pomodoro1.name() << " != " << std::endl;
        return false;
    }
    if (!timeSpan_equals_to_minute(pomodoro1.timeSpan(),
                                   pomodoro2.timeSpan())) {
        std::cout << pomodoro1.startTime() << " - " << pomodoro1.finishTime()
                  << " != " << pomodoro2.startTime() << " - "
                  << pomodoro2.finishTime() << std::endl;
        return false;
    }

    // Tags are compared sorted, because there is no guarantee of tag
    // ordering
    std::list<std::string> tags1 = pomodoro1.tags();
    std::list<std::string> tags2 = pomodoro2.tags();
    tags1.sort();
    tags2.sort();

    return tags1 == tags2;
}
}
;

TEST(PomodoroModel, test_insert_and_delete)
{
    PomodoroModel pomodoroModel;
    TimeSpan timeSpan{std::chrono::system_clock::now(),
                      std::chrono::system_clock::now()};
    // TODO fix when streamlined time zones issues
    // For now, the system stores time in local time zone, so need to
    // compare with local time
    TimeSpan expectedInterval{DateTime::currentDateTimeLocal(),
                              DateTime::currentDateTimeLocal()};

    TaskModel todoItemModel;
    std::string name{"Test item"};
    std::list<std::string> tags{"Tag1", "Tag2"};
    int estimatedPomodoros{4};
    int spentPomodoros{0};
    TodoItem item{name, estimatedPomodoros, spentPomodoros, tags, false};
    Pomodoro expectedPomodoro{item, expectedInterval};

    CHECK(todoItemModel.insert(item));
    auto todoId = todoItemModel.itemIdAt(0);

    CHECK(pomodoroModel.insert(todoId, timeSpan));
    CHECK(pomodoro_equal(expectedPomodoro, pomodoroModel.itemAt(0)));

    // Check that spent pomodoros have been incremented
    todoItemModel.select();
    TodoItem insertedTodo = todoItemModel.itemAt(0);
    CHECK_EQUAL(spentPomodoros + 1, insertedTodo.spentPomodoros());

    CHECK(pomodoroModel.remove(0));

    CHECK_EQUAL(0, pomodoroModel.numRecords());

    // Check that spent pomodoros have been decremented
    todoItemModel.select();
    insertedTodo = todoItemModel.itemAt(0);
    CHECK_EQUAL(spentPomodoros, insertedTodo.spentPomodoros());
}

TEST(PomodoroModel, test_deleting_todo_item_remove_all_associated_pomodoros)
{
    PomodoroModel pomodoroModel;
    TimeSpan timeSpan{std::chrono::system_clock::now(),
                      std::chrono::system_clock::now()};
    TaskModel todoItemModel;
    std::string name{"Test item"};
    std::list<std::string> tags{"Tag1", "Tag2"};
    int estimatedPomodoros{4};
    int spentPomodoros{0};
    TodoItem item{name, estimatedPomodoros, spentPomodoros, tags, false};

    CHECK(todoItemModel.insert(item));
    auto todoId = todoItemModel.itemIdAt(0);

    int numInsertedPomos{10};
    for (int i = 0; i < numInsertedPomos; ++i) {
        CHECK(pomodoroModel.insert(todoId, timeSpan));
    }

    CHECK_EQUAL(numInsertedPomos, pomodoroModel.numRecords());
    // As inserting pomodoros increments todo_item spent_pomodoros, model needs
    // to
    // be refreshed
    todoItemModel.select();

    CHECK(todoItemModel.remove(0));
    CHECK_EQUAL(0, todoItemModel.numRecords());
    pomodoroModel.select();
    CHECK_EQUAL(0, pomodoroModel.numRecords());
}
