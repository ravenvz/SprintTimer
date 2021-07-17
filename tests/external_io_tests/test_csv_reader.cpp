/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#include "external_io/CSVReader.h"
#include "gtest/gtest.h"
#include <iostream>
#include <iterator>
#include <sstream>

using namespace sprint_timer::external_io;

namespace {

// TODO that makes test run depend on working directory
constexpr std::string_view rfc_4180_test_file =
    "tests/external_io_tests/data/rfc_4180.csv";

} // namespace

TEST(TestCSV, test_trows_exception_when_file_not_found)
{
    std::fstream file{"bogus_file_name.csv"};

    ASSERT_THROW((CSVReader{file}), StreamReadError);
}

TEST(TestCSV, test_throws_exception_when_invalid_file_format)
{
    std::istringstream stream{"One,two,three\none,two,three,four"};

    ASSERT_THROW((CSVReader{stream}), FormatError);
}

TEST(TestCSV, test_handles_case_wnen_last_record_missing_ending_line_break)
{
    std::istringstream stream{"one,two,three\none,two,three"};
    std::vector<Row> expected{{"one", "two", "three"}, {"one", "two", "three"}};

    CSVReader reader{stream};

    EXPECT_TRUE(std::equal(
        expected.cbegin(), expected.cend(), reader.cbegin(), reader.cend()));
}

TEST(TestCSV, test_reads_RFC_4180_file)
{
    std::fstream file{std::string{rfc_4180_test_file}};
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

    EXPECT_TRUE(std::equal(expected_data.cbegin(),
                           expected_data.cend(),
                           reader.cbegin(),
                           reader.cend()));
}
