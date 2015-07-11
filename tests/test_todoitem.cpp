#include "core/entities.h"
#include <TestHarness.h>


TEST_GROUP(TodoItemGroup) {

};

TEST(TodoItemGroup, test_description_all_parts_present) {
    TodoItem item {"#Test All parts present *5"};

    CHECK_TRUE(QStringList {"Test"} == item.tags)
    CHECK_TRUE(QString {"All parts present"} == item.name)
    CHECK_EQUAL(5, item.estimatedPomodoros)
}

TEST(TodoItemGroup, test_desciption_no_estimated) {
    TodoItem item {"#Test Todo with tag"};

    CHECK_TRUE(QStringList {"Test"} == item.tags)
    CHECK_TRUE(QString {"Todo with tag"} == item.name)
    CHECK_EQUAL(0, item.estimatedPomodoros)
}

TEST(TodoItemGroup, test_description_no_tags) {
    TodoItem item {"Simple todo *2"};

    CHECK_TRUE(QString {"Simple todo"} == item.name)
    CHECK_EQUAL(2, item.estimatedPomodoros)
    CHECK_TRUE(item.tags.empty())
}

TEST(TodoItemGroup, test_description_no_name) {
    TodoItem item {"#Tag #Test *4"};

    QStringList expectedTags {"Tag", "Test"};
    CHECK_TRUE(expectedTags == item.tags)
    CHECK_EQUAL(4, item.estimatedPomodoros)
}

TEST(TodoItemGroup, test_description_only_last_num_estimated_should_be_considered) {
    TodoItem item {"Multiple estimated *4 *9"};

    CHECK_EQUAL(9, item.estimatedPomodoros)
}

// TEST(TodoItemGroup, test_encode_all_parts_present) {
//     TodoItem item {"Todo name", 4, 3, 0, QStringList {"Test"}, false, 123};
//
//     CHECK_TRUE(QString {"#Test Todo name (3/4)"} == item.description)
// }
    //
    // def test_encode_description_no_tags(self):
    //     name = "Todo name"
    //     num_estimated = 4
    //
    //     todo = TodoItem.from_parts(name, None, num_estimated)
    //
    //     self.assertEquals("Todo name (0/4)", todo.description)
