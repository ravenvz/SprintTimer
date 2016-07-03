#include "qt_storage_impl/utils.h"
#include <list>
#include <TestHarness.h>


TEST_GROUP(TestTwoWayDiff){
};

TEST(TestTwoWayDiff, test_diff_left_and_right)
{
    std::list<std::string> oldTags{"Tag1", "Tag2", "Tag3"};
    std::list<std::string> newTags{"Tag2", "Tag3", "Tag4"};
    std::list<std::string> expectedRemove{"Tag1"};
    std::list<std::string> expectedInsert{"Tag4"};
    std::list<std::string> tagsToRemove;
    std::list<std::string> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    CHECK(expectedRemove == tagsToRemove);
    CHECK(expectedInsert == tagsToInsert);
}

TEST(TestTwoWayDiff, test_diff_left_empty)
{
    std::list<std::string> oldTags{};
    std::list<std::string> newTags{"Tag2", "Tag3", "Tag4"};
    std::list<std::string> expectedRemove{};
    std::list<std::string> expectedInsert{"Tag2", "Tag3", "Tag4"};
    std::list<std::string> tagsToRemove;
    std::list<std::string> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    CHECK(expectedRemove == tagsToRemove);
    CHECK(expectedInsert == tagsToInsert);
}

TEST(TestTwoWayDiff, test_diff_right_empty)
{
    std::list<std::string> oldTags{"Tag2", "Tag3", "Tag4"};
    std::list<std::string> newTags{};
    std::list<std::string> expectedRemove{"Tag2", "Tag3", "Tag4"};
    std::list<std::string> expectedInsert{};
    std::list<std::string> tagsToRemove;
    std::list<std::string> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    CHECK(expectedRemove == tagsToRemove);
    CHECK(expectedInsert == tagsToInsert);
}

TEST(TestTwoWayDiff, test_all_unique)
{
    std::list<std::string> oldTags{"Tag1", "Tag2", "Tag3"};
    std::list<std::string> newTags{"Tag4", "Tag5", "Tag6"};
    std::list<std::string> expectedRemove{"Tag1", "Tag2", "Tag3"};
    std::list<std::string> expectedInsert{"Tag4", "Tag5", "Tag6"};
    std::list<std::string> tagsToRemove;
    std::list<std::string> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    CHECK(expectedRemove == tagsToRemove);
    CHECK(expectedInsert == tagsToInsert);
}
