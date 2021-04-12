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
#include "gtest/gtest.h"
#include "core/SprintBuilder.h"
#include "external_io/SprintToCsvAlgorithm.h"

using namespace sprint_timer;
using namespace std::chrono_literals;
using namespace ::sprint_timer::entities;
using namespace ::dw;

class SprintToCsvFixture : public ::testing::Test {
public:
    Date someDate{Year{2020}, Month{6}, Day{19}};
    DateTime startTime{DateTime{someDate} + 2h};
    SprintBuilder builder;
    external_io::SprintToCsvAlgorithm sprintSerializer;
};

TEST_F(SprintToCsvFixture, serializes_sprint)
{
    const auto sprint =
        builder.withName("Alone sprint!")
            .withTaskUuid("123")
            .withUuid("321")
            .withExplicitTags({Tag{"SomeTag"}, Tag{"SomeOtherTag"}})
            .withTimeSpan(
                DateTimeRange{startTime + 10h, startTime + 10h + 25min})
            .build();
    const std::string expected{"SomeTag,SomeOtherTag;19.06.2020 12:00 - "
                               "19.06.2020 12:25;Alone sprint!;123;321"};

    EXPECT_EQ(expected, sprintSerializer.serialize(sprint));
}

TEST_F(SprintToCsvFixture, serializes_batch)
{
    std::vector<Sprint> sprints;
    sprints.push_back(
        builder.withName("Sprint 1")
            .withTaskUuid("123")
            .withUuid("321")
            .withExplicitTags({Tag{"SomeTag"}, Tag{"SomeOtherTag"}})
            .withTimeSpan(DateTimeRange{startTime, startTime + 25min})
            .build());
    sprints.push_back(
        builder.withName("Sprint 2")
            .withTaskUuid("345")
            .withUuid("534")
            .withExplicitTags({})
            .withTimeSpan(DateTimeRange{startTime + 2h, startTime + 2h + 25min})
            .build());
    const std::vector<std::string> expected{
        "SomeTag,SomeOtherTag;19.06.2020 02:00 - 19.06.2020 02:25;Sprint "
        "1;123;321",
        ";19.06.2020 04:00 - 19.06.2020 04:25;Sprint 2;345;534"};

    EXPECT_EQ(expected, sprintSerializer.serializeBatch(sprints));
}
