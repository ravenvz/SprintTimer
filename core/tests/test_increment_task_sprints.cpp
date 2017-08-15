/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

// TODO remove when Gtest drops std::tr1
// Workaround for C++17 as std::tr1 no longer available and Gtest uses it
#define GTEST_LANG_CXX11 1

#include "fixtures/FakeTaskStorageWriter.h"
#include "core/use_cases/IncrementTaskSprints.h"
#include "gtest/gtest.h"

namespace {

Task defaultItem{
    "Item name", 4, 2, {Tag{"Tag 1"}, Tag{"Tag 2"}}, false};

} // namespace

TEST(IncrementTaskSprints, test_execute_and_undo)
{
    FakeStorage<Task> storage;
    FakeTaskStorageWriter writer{storage};
    writer.save(defaultItem);
    std::string uuid = defaultItem.uuid();

    UseCases::IncrementTaskSprints increment{writer, uuid};
    increment.execute();

    EXPECT_EQ(3, (*writer.storage.getItem(uuid)).actualCost());

    increment.undo();

    EXPECT_EQ(2, (*writer.storage.getItem(uuid)).actualCost());
}

TEST(IncrementTaskSprints, test_should_not_undo_if_was_not_executed)
{
    FakeStorage<Task> storage;
    FakeTaskStorageWriter writer{storage};
    writer.save(defaultItem);
    std::string uuid = defaultItem.uuid();

    UseCases::IncrementTaskSprints increment{writer, uuid};

    EXPECT_EQ(2, (*writer.storage.getItem(uuid)).actualCost());

    increment.undo();

    EXPECT_EQ(2, (*writer.storage.getItem(uuid)).actualCost());
}
