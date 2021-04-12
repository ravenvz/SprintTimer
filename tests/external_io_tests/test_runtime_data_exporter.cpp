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
#include "mocks/SerializationAlgorithmMock.h"
#include "mocks/SinkMock.h"
#include "external_io/RuntimeConfigurableDataExporter.h"

using namespace sprint_timer;
using namespace sprint_timer::external_io;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

class RuntimeConfigurableDataExporterFixture : public ::testing::Test {
public:
    NiceMock<mocks::SerializationAlgorithmMock<int>> csvSerializationAlgo;
    NiceMock<mocks::SerializationAlgorithmMock<int>> jsonSerializationAlgo;
    NiceMock<mocks::SinkMock> stdoutSink;
    NiceMock<mocks::SinkMock> networkSink;
    NiceMock<mocks::SinkMock> fileSink;
};

TEST_F(RuntimeConfigurableDataExporterFixture, dispatches_data)
{
    Serializer<int> serializer{
        Serializer<int>::AlgoMap{{DataFormat::Csv, csvSerializationAlgo},
                                 {DataFormat::Json, jsonSerializationAlgo}}};
    RuntimeSinkRouter sinkRouter{
        RuntimeSinkRouter::SinkTypeMapper{{SinkType::Network, networkSink},
                                          {SinkType::Stdout, stdoutSink},
                                          {SinkType::File, fileSink}}};
    RuntimeConfigurableDataExporter<int> exporter{serializer, sinkRouter};
    std::vector<std::string> expected{"Serialized", "Serialized", "Serialized"};
    ON_CALL(jsonSerializationAlgo, serialize(_))
        .WillByDefault(Return("Serialized"));

    EXPECT_CALL(networkSink, send(std::move(expected)));

    exporter.exportData(
        std::vector<int>{1, 2, 3}, DataFormat::Json, SinkType::Network);
}

TEST_F(RuntimeConfigurableDataExporterFixture,
       throws_when_sink_dispatching_fails)
{
    Serializer<int> serializer{
        Serializer<int>::AlgoMap{{DataFormat::Csv, csvSerializationAlgo},
                                 {DataFormat::Json, jsonSerializationAlgo}}};
    RuntimeSinkRouter sinkRouter{RuntimeSinkRouter::SinkTypeMapper{
        {SinkType::Network, networkSink}, {SinkType::File, fileSink}}};
    RuntimeConfigurableDataExporter<int> exporter{serializer, sinkRouter};

    ASSERT_THROW(exporter.exportData(std::vector<int>{1, 2, 3},
                                     DataFormat::Csv,
                                     SinkType::Stdout),
                 SinkRoutingException);
}

TEST_F(RuntimeConfigurableDataExporterFixture,
       throws_when_serializer_dispatching_fails)
{
    Serializer<int> serializer{
        Serializer<int>::AlgoMap{{DataFormat::Csv, csvSerializationAlgo}}};
    RuntimeSinkRouter sinkRouter{
        RuntimeSinkRouter::SinkTypeMapper{{SinkType::Network, networkSink},
                                          {SinkType::Stdout, stdoutSink},
                                          {SinkType::File, fileSink}}};
    RuntimeConfigurableDataExporter<int> exporter{serializer, sinkRouter};

    ASSERT_THROW(exporter.exportData(std::vector<int>{1, 2, 3},
                                     DataFormat::Json,
                                     SinkType::Network),
                 SerializationException);
}
