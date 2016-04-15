#include "utils/StringUtils.h"
#include <TestHarness.h>
#include <iostream>
#include <iterator>


TEST_GROUP(TestStringUtils){

};

TEST(TestStringUtils, test_parse_words)
{
    std::string text{"Some    text1, that! should Be parsed..."};
    std::list<std::string> expected{
        "Some", "text1", "that", "should", "Be", "parsed"};

    CHECK(expected == StringUtils::parseWords(text));
}
