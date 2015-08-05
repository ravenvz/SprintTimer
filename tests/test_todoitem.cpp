#include "core/entities.h"
#include <TestHarness.h>


TEST_GROUP(TodoItemGroup) {

};

TEST(TodoItemGroup, test_description_all_parts_present) {
    QString encodedDescription {"#Test All parts present *5"};
    TodoItem item {encodedDescription};

    CHECK_TRUE(QStringList {"Test"} == item.tags)
    CHECK_TRUE(QString {"All parts present"} == item.name)
    CHECK_EQUAL(5, item.estimatedPomodoros)
}

TEST(TodoItemGroup, test_description_should_set_estimated_pomodoros_to_one_if_no_estimated) {
    QString encodedDescription {"#Test Todo with tag"};
    TodoItem item {encodedDescription};

    CHECK_TRUE(QStringList {"Test"} == item.tags)
    CHECK_TRUE(QString {"Todo with tag"} == item.name)
    CHECK_EQUAL(1, item.estimatedPomodoros)
}

TEST(TodoItemGroup, test_description_no_tags) {
    QString encodedDescription {"Simple todo *2"};
    TodoItem item {encodedDescription};

    CHECK_TRUE(QString {"Simple todo"} == item.name)
    CHECK_EQUAL(2, item.estimatedPomodoros)
    CHECK_TRUE(item.tags.empty())
}

TEST(TodoItemGroup, test_description_no_name) {
    QString encodedDescription {"#Tag #Test *4"};
    TodoItem item {encodedDescription};

    QStringList expectedTags {"Tag", "Test"};
    CHECK_TRUE(expectedTags == item.tags)
    CHECK_EQUAL(4, item.estimatedPomodoros)
}

TEST(TodoItemGroup, test_description_only_last_num_estimated_should_be_considered) {
    QString encodedDescription {"Multiple estimated *4 *9"};
    TodoItem item {encodedDescription};

    CHECK_EQUAL(9, item.estimatedPomodoros)
}

TEST(TodoItemGroup, test_only_threats_words_preceeded_by_single_hash_as_tags) {
    QString encodedDescription {"##My #tag1  #   ##    beautiful,marvelous, great   content"};
    TodoItem item {encodedDescription};

    CHECK_TRUE(QStringList {"tag1"} == item.tags)
    CHECK_TRUE(QString {"##My # ## beautiful,marvelous, great content"} == item.name)
}

