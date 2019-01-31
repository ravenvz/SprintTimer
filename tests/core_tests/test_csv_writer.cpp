/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#include "core/utils/CSVEncoder.h"
#include "gtest/gtest.h"

using namespace sprint_timer::external_io;
using namespace sprint_timer::utils;

TEST(TestCSVEncoder, test_trivial_encoding)
{
    std::vector<CSVEncoder::Data> data{{"One", "two", "three"}};
    std::string expected{"One,two,three\n"};
    CSVEncoder encoder;

    EXPECT_EQ(expected, encoder.encode(data));
}

TEST(TestCSVEncoder, test_quoted_values_encoding)
{
    std::vector<CSVEncoder::Data> data{{"One", "this \"value\" is quoted", "three"}};
    std::string expected{"One,this \"\"value\"\" is quoted,three\n"};
    CSVEncoder encoder;

    EXPECT_EQ(expected, encoder.encode(data));
}

TEST(TestCSVEncoder, test_encode_generic_type)
{
    struct T {
        std::string name;
        int value;
    };
    std::vector<T> data{T{"Pressure", 42}, T{"Temperature", 22}};
    auto vectorize = [](const T& item) {
        std::vector<std::string> vec;
        vec.push_back(item.name);
        vec.push_back(std::to_string(item.value));
        return vec;
    };
    CSVEncoder encoder;
    std::string expected{"Pressure,42\nTemperature,22\n"};

    EXPECT_EQ(expected, encoder.encode(data, vectorize));
}
