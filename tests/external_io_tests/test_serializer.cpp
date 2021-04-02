/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "gtest/gtest.h"
#include <external_io/Serializer.h>

using namespace sprint_timer;
using namespace sprint_timer::external_io;
using ::testing::NiceMock;

class SerializerFixture : public ::testing::Test {
public:
    NiceMock<mocks::SerializationAlgorithmMock<int>>
        firstSerializationAlgorithm;
    NiceMock<mocks::SerializationAlgorithmMock<int>>
        secondSerializationAlgorithm;
    Serializer<int> serializer{
        {{DataFormat::Csv, firstSerializationAlgorithm},
         {DataFormat::Json, secondSerializationAlgorithm}}};
};

TEST_F(SerializerFixture,
       delegates_serialization_to_specific_algorithm_based_on_runtime_parameter)
{
    EXPECT_CALL(firstSerializationAlgorithm, serialize(47));

    serializer.serialize(47, DataFormat::Csv);

    EXPECT_CALL(secondSerializationAlgorithm, serialize(78));

    serializer.serialize(78, DataFormat::Json);
}

TEST_F(SerializerFixture, throws_when_unable_to_find_preconfigured_algorithm)
{
    Serializer<int> misconfiguredSerializer{
        {{DataFormat::Csv, firstSerializationAlgorithm}}};

    ASSERT_THROW(misconfiguredSerializer.serialize(42, DataFormat::Json),
                 SerializationException);
}
