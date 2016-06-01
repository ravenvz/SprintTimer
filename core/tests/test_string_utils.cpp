#include "core/StringUtils.h"
#include <TestHarness.h>


TEST_GROUP(TestStringUtils){

};

TEST(TestStringUtils, test_parse_words)
{
    std::string text{"Some    text1, that! should Be parsed..."};
    std::list<std::string> expected{
        "Some", "text1", "that", "should", "Be", "parsed"};

    CHECK(expected == StringUtils::parseWords(text));
}

TEST(TestStringUtils, test_join_with_some_parts_empty)
{
    std::vector<std::string> parts{"Some", "", "are", "empty"};
    std::string expected{"Some are empty"};

    std::string actual = StringUtils::join(parts, " ");

    CHECK_EQUAL(expected.size(), actual.size())
    CHECK(std::equal(expected.begin(), expected.end(), actual.begin()));
}

TEST(TestStringUtils, test_join_with_all_parts_empty)
{
    std::vector<std::string> parts{"", "", "", ""};
    std::string expected{""};

    std::string actual = StringUtils::join(parts, " ");

    CHECK_EQUAL(expected.size(), actual.size())
    CHECK(std::equal(expected.begin(), expected.end(), actual.begin()));
}

TEST(TestStringUtils, test_join_on_list_of_ints)
{
    std::list<int> parts{1, 2, 3, 4};
    std::string expected{"1 -> 2 -> 3 -> 4"};

    std::string actual
        = StringUtils::join(parts.cbegin(), parts.cend(), " -> ");

    CHECK_EQUAL(expected.size(), actual.size())
    CHECK(std::equal(expected.begin(), expected.end(), actual.begin()));
}
