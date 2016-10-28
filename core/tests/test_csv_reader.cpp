#include "core/utils/CSVReader.h"
#include <TestHarness.h>
#include <iostream>
#include <iterator>
#include <sstream>

using namespace CSV;

TEST_GROUP(TestCSV){

};

TEST(TestCSV, test_trows_exception_when_file_not_found)
{
    std::fstream file{"bogus_file_name.csv"};
    CHECK_THROWS(StreamReadError, (CSVReader{file}));
}

TEST(TestCSV, test_throws_exception_when_invalid_file_format)
{
    std::istringstream stream{"One,two,three\none,two,three,four"};
    CHECK_THROWS(FormatError, (CSVReader{stream}));
}

TEST(TestCSV, test_handles_case_wnen_last_record_missing_ending_line_break) {
    std::istringstream stream{"one,two,three\none,two,three"};
    std::vector<Row> expected{{"one", "two", "three"},
                              {"one", "two", "three"}};

    CSVReader reader{stream};

    CHECK(std::equal(expected.cbegin(), expected.cend(), reader.cbegin()));
}

TEST(TestCSV, test_reads_RFC_4180_file)
{
    std::fstream file{"../core/tests/data/rfc_4180.csv"};
    CSVReader reader{file};
    Row expected_header{"Year", "Make", "Model", "Description", "Price"};
    std::vector<Row> expected_data{
        Row{"Year", "Make", "Model", "Description", "Price"},
        Row{"1997", "Ford", "E350", "ac, abs, moon", "3000.00"},
        Row{"1999", "Chevy", "Venture \"Extended Edition\"", "", "4900.00"},
        Row{"1999",
            "Chevy",
            "Venture \"Extended Edition, Very Large\"",
            "",
            "5000.00"},
        Row{"1996",
            "Jeep",
            "Grand Cherokee",
            "MUST SELL!\nair, moon roof, loaded",
            "4799.00"}};

    CHECK(std::equal(
        expected_data.cbegin(), expected_data.cend(), reader.cbegin()));
}
