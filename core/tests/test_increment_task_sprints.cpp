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
#include "fixtures/FakeTaskStorageWriter.h"
#include "use_cases/IncrementTaskSprints.h"
#include <TestHarness.h>


TEST_GROUP(IncrementTaskSprints)
{
    Task defaultItem{
        "Item name", 4, 2, {Tag{"Tag 1"}, Tag{"Tag 2"}}, false};
};

TEST(IncrementTaskSprints, test_execute_and_undo)
{
    FakeStorage<Task> storage;
    FakeTaskStorageWriter writer{storage};
    writer.save(defaultItem);
    std::string uuid = defaultItem.uuid();

    UseCases::IncrementTaskSprints increment{writer, uuid};
    increment.execute();

    CHECK_EQUAL(3, writer.storage.getItem(uuid).value().spentPomodoros());

    increment.undo();

    CHECK_EQUAL(2, writer.storage.getItem(uuid).value().spentPomodoros());
}

TEST(IncrementTaskSprints, test_should_not_undo_if_was_not_executed)
{
    FakeStorage<Task> storage;
    FakeTaskStorageWriter writer{storage};
    writer.save(defaultItem);
    std::string uuid = defaultItem.uuid();

    UseCases::IncrementTaskSprints increment{writer, uuid};

    CHECK_EQUAL(2, writer.storage.getItem(uuid).value().spentPomodoros());

    increment.undo();

    CHECK_EQUAL(2, writer.storage.getItem(uuid).value().spentPomodoros());
}
