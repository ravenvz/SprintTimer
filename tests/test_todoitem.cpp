#include "core/entities/TodoItem.h"
#include <TestHarness.h>


TEST_GROUP(TodoItemGroup){

};

TEST(TodoItemGroup, test_description_all_parts_present)
{
    TodoItem item{"#Test All parts present *5"};

    CHECK_TRUE(std::list<std::string>{"Test"} == item.tags())
    CHECK_EQUAL(std::string{"All parts present"}, item.name())
    CHECK_EQUAL(5, item.estimatedPomodoros())
}

TEST(TodoItemGroup,
     test_description_should_set_estimated_pomodoros_to_one_if_no_estimated)
{
    TodoItem item{"#Test Todo with tag"};

    CHECK_TRUE(std::list<std::string>{"Test"} == item.tags())
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
IGNORE_TEST(TodoItemGroup, test_description_no_name)
{
    TodoItem item{"#Tag #Test *4"};

    std::list<std::string> expectedTags{"Tag", "Test"};
    CHECK_TRUE(expectedTags == item.tags())
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

    CHECK_TRUE(std::list<std::string>{"tag1"} == item.tags())
    CHECK_EQUAL(std::string{"##My # ## beautiful,marvelous, great content"},
                item.name())
}
