#include "core/entities/TodoItem.h"
#include <TestHarness.h>

#include <iostream>

TEST_GROUP(TodoItemGroup){

};

TEST(TodoItemGroup, test_description_all_parts_present)
{
    TodoItem item{"#Test All parts present *5"};

    CHECK_TRUE(std::list<Tag>{Tag{"Test"}} == item.tags())
    CHECK_EQUAL(std::string{"All parts present"}, item.name())
    CHECK_EQUAL(5, item.estimatedPomodoros())
}

TEST(TodoItemGroup,
     test_description_should_set_estimated_pomodoros_to_one_if_no_estimated)
{
    TodoItem item{"#Test Todo with tag"};

    CHECK_TRUE(std::list<Tag>{Tag{"Test"}} == item.tags())
    CHECK_EQUAL(std::string{"Todo with tag"}, item.name())
    CHECK_EQUAL(1, item.estimatedPomodoros())
}

TEST(TodoItemGroup, test_description_no_tags)
{
    TodoItem item{"Simple todo *2"};

    CHECK_EQUAL(std::string{"Simple todo"}, item.name())
    CHECK_EQUAL(2, item.estimatedPomodoros())
    CHECK_TRUE(item.tags().empty())
}

// TODO figure out what the heck is wrong with this
TEST(TodoItemGroup, test_description_no_name)
{
    TodoItem item{"#Tag #Test *4"};

    std::list<Tag> expected{Tag{"Tag"}, Tag{"Test"}};

    auto actual = item.tags();

    CHECK_EQUAL(expected.size(), actual.size())
    CHECK(std::equal(expected.begin(), expected.end(), actual.begin()));
    CHECK_EQUAL(4, item.estimatedPomodoros())
}

TEST(TodoItemGroup,
     test_description_only_last_num_estimated_should_be_considered)
{
    TodoItem item{"Multiple estimated *4 *9"};

    CHECK_EQUAL(9, item.estimatedPomodoros())
}

TEST(TodoItemGroup, test_only_threats_words_preceeded_by_single_hash_as_tags)
{
    TodoItem item{"##My #tag1  #   ##    beautiful,marvelous, great   content"};

    CHECK_TRUE(std::list<Tag>{Tag{"tag1"}} == item.tags())
    CHECK_EQUAL(std::string{"##My # ## beautiful,marvelous, great content"},
                item.name())
}

TEST(TodoItemGroup, test_to_string)
{
    // TodoItem item{"I am item with no tags"};
    TodoItem item{"I am item with no tags",
                  4,
                  2,
                  "uuid",
                  std::list<Tag>(),
                  false,
                  DateTime::currentDateTimeLocal()};
    std::string expected{"I am item with no tags 2/4"};

    std::string actual = item.toString();

    CHECK_EQUAL(expected.size(), actual.size())
    CHECK(std::equal(expected.begin(), expected.end(), actual.begin()));
}
