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
#include "external_io/RuntimeSinkRouter.h"
#include "mocks/SinkMock.h"

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

    const std::vector<std::string> fileSinkData{"whatever", "file"};
    EXPECT_CALL(fileSinkMock, send(::testing::ElementsAreArray(fileSinkData)));
    router.route(fileSinkData, SinkType::File);

    const std::vector<std::string> stdoutSinkData{"whatever", "stdout"};
    EXPECT_CALL(stdoutSinkMock,
                send(::testing::ElementsAreArray(stdoutSinkData)));
    router.route(stdoutSinkData, SinkType::Stdout);

    const std::vector<std::string> networkSinkData{"whatever", "network"};
    EXPECT_CALL(networkSinkMock,
                send(::testing::ElementsAreArray(networkSinkData)));
    router.route(networkSinkData, SinkType::Network);
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
