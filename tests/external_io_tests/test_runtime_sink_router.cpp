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
#include "mocks/SinkMock.h"
#include "external_io/RuntimeSinkRouter.h"

using ::testing::NiceMock;
using namespace sprint_timer;
using namespace sprint_timer::external_io;

TEST(RuntimeSinkRouterFixture, delegates_to_sink_based_on_runtime_parameter)
{
    NiceMock<mocks::SinkMock> fileSinkMock;
    NiceMock<mocks::SinkMock> stdoutSinkMock;
    NiceMock<mocks::SinkMock> networkSinkMock;
    RuntimeSinkRouter::SinkTypeMapper mapper{
        {SinkType::File, fileSinkMock},
        {SinkType::Network, networkSinkMock},
        {SinkType::Stdout, stdoutSinkMock}};
    RuntimeSinkRouter router{std::move(mapper)};

    EXPECT_CALL(fileSinkMock,
                send(std::vector<std::string>{"whatever", "file"}));
    router.route(std::vector<std::string>{"whatever", "file"}, SinkType::File);

    EXPECT_CALL(stdoutSinkMock,
                send(std::vector<std::string>{"whatever", "stdout"}));
    router.route(std::vector<std::string>{"whatever", "stdout"},
                 SinkType::Stdout);

    EXPECT_CALL(networkSinkMock,
                send(std::vector<std::string>{"whatever", "network"}));
    router.route(std::vector<std::string>{"whatever", "network"},
                 SinkType::Network);
}

TEST(RuntimeSinkRouterFixture, throws_when_unable_route_to_sink)
{
    NiceMock<mocks::SinkMock> fileSinkMock;
    RuntimeSinkRouter::SinkTypeMapper mapper{{SinkType::File, fileSinkMock}};
    RuntimeSinkRouter router{std::move(mapper)};

    ASSERT_THROW(
        router.route(std::vector<std::string>{"whatever"}, SinkType::Network),
        SinkRoutingException);
}
