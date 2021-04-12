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

#include "external_io/CsvSerializationAlgorithm.h"
#include "gtest/gtest.h"

using namespace sprint_timer::external_io;

TEST(TestCsvSerializationAlgorithm, handles_trivial_encoding)
{
    const std::vector<std::string> data{"One", "two", "three"};
    std::string expected{"One,two,three"};

    EXPECT_EQ(expected, encode(data, ','));
}

TEST(TestCsvSerializationAlgorithm, handles_quoted_values_encoding)
{
    const std::vector<std::string> data{
        "One", "this \"value\" is quoted", "three"};
    std::string expected{"One;this \"\"value\"\" is quoted;three"};

    EXPECT_EQ(expected, encode(data, ';'));
}
