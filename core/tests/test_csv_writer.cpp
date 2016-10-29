/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "core/external_io/CSVWriter.h"
#include "core/external_io/OstreamSink.h"
#include <TestHarness.h>

using namespace ExternalIO;

TEST_GROUP(TestCSVWriter){

};

TEST(TestCSVWriter, test_encodes_trivial)
{
    std::stringstream expected;
    std::vector<std::string> data{"One", "two", "three"};
    expected << "One,two,three\n";
    auto actual = std::make_shared<std::stringstream>();
    auto sink = OstreamSink{actual};
    CSV::CSVWriter writer;

    writer.exportData(&sink, data);

    CHECK_EQUAL(expected.str(), (*actual).str())
}

TEST(TestCSVWriter, test_encodes_quoted_values)
{
    std::stringstream expected;
    std::vector<std::string> data{"One", "this \"value\" is quoted", "three"};
    expected << "One,this \"\"value\"\" is quoted,three\n";
    auto actual = std::make_shared<std::stringstream>();
    auto sink = OstreamSink{actual};
    CSV::CSVWriter writer;

    writer.exportData(&sink, data);

    CHECK_EQUAL(expected.str(), (*actual).str());
}
