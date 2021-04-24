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

#include "core/entities/Task.h"
#include "gtest/gtest.h"

using namespace sprint_timer::entities;
using dw::DateTime;

TEST(TestTask, ostream_operator)
{
    using namespace dw;
    using namespace std::chrono_literals;

    constexpr auto modificationStamp =
        DateTime{Date{Year{2016}, Month{9}, Day{21}}} + 12h + 59min + 19s;
    const Task item{"I am item with no tags",
                    4,
                    2,
                    "uuid",
                    std::list<Tag>(),
                    false,
                    modificationStamp};
    const auto uuid = item.uuid();
    std::string expected{"I am item with no tags 2/4"};
    expected += " Uuid: ";
    expected += uuid;
    expected += " ";
    expected += "21.09.2016 12:59:19";

    std::stringstream ss;
    ss << item;
    std::string actual{ss.str()};

    EXPECT_EQ(expected, actual);
}
