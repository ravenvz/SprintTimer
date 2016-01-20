#include "gui/pomodoromodel.h"
#include "gui/todoitemmodel.h"
#include <TestHarness.h>

#include <QSqlRecord>


TEST_GROUP(PomodoroModel) {

    void setup() {
        // Memory leak checker does not play well with Qt objects at all.
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
        QSqlQuery query;
        query.exec("delete from pomodoro;");
        query.exec("delete from todo_item");
        query.exec("delete from todotag");
        query.exec("delete from tag");
    }

    void teardown() {
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    }

    bool pomodoro_equal(const Pomodoro& pomodoro1, const Pomodoro& pomodoro2) {
        return pomodoro1.name() == pomodoro2.name() &&
               pomodoro1.startTime() == pomodoro2.startTime() &&
               pomodoro1.finishTime() == pomodoro2.finishTime() &&
               pomodoro1.tags() == pomodoro2.tags();
    }

};

TEST(PomodoroModel, test_insert_and_delete) {
    PomodoroModel pomodoroModel;
    TimeInterval interval {QDateTime::currentDateTime(), QDateTime::currentDateTime()};

    TodoItemModel todoItemModel;
    QString name {"Test item"};
    QStringList tags {"Tag1", "Tag2"};
    unsigned estimatedPomodoros {4};
    unsigned spentPomodoros {0};
    TodoItem item {name, estimatedPomodoros, spentPomodoros, tags, false};
    Pomodoro expectedPomodoro {name, interval, tags};

    CHECK(todoItemModel.insert(item));
    auto todoId = todoItemModel.itemIdAt(0);

    CHECK(pomodoroModel.insert(todoId, interval));
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

TEST(PomodoroModel, test_deleting_todo_item_remove_all_associated_pomodoros) {
    PomodoroModel pomodoroModel;
    TimeInterval interval {QDateTime::currentDateTime(), QDateTime::currentDateTime()};
    TodoItemModel todoItemModel;
    QString name {"Test item"};
    QStringList tags {"Tag1", "Tag2"};
    unsigned estimatedPomodoros {4};
    unsigned spentPomodoros {0};
    TodoItem item {name, estimatedPomodoros, spentPomodoros, tags, false};

    CHECK(todoItemModel.insert(item));
    auto todoId = todoItemModel.itemIdAt(0);

    int numInsertedPomos {10};
    for (int i = 0; i < numInsertedPomos; ++i) {
        CHECK(pomodoroModel.insert(todoId, interval));
    }

    CHECK_EQUAL(numInsertedPomos, pomodoroModel.numRecords());
    // As inserting pomodoros increments todo_item spent_pomodoros, model needs to
    // be refreshed
    todoItemModel.select();

    CHECK(todoItemModel.remove(0));
    CHECK_EQUAL(0, todoItemModel.numRecords());
    pomodoroModel.select();
    CHECK_EQUAL(0, pomodoroModel.numRecords());
}


