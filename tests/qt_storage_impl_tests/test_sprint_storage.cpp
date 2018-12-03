/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "QtStorageInitializer.h"
#include "gtest/gtest.h"

using sprint_timer::ISprintStorageReader;
using sprint_timer::ITaskStorageReader;
using sprint_timer::SprintBuilder;
using sprint_timer::TaskBuilder;
using sprint_timer::entities::Sprint;
using sprint_timer::entities::Task;

class SprintStorageFixture : public ::testing::Test {
public:
};

TEST_F(SprintStorageFixture, inserting_sprint_increments_task_actual_count)
{
    QtStorageInitializer initializer{"spssr.db"};
    const Task someTask = TaskBuilder{}.withActualCost(4).build();
    const Task expectedTask
        = TaskBuilder{}
              .withUuid(someTask.uuid())
              .withActualCost(5)
              .withLastModificationStamp(someTask.lastModified())
              .build();
    const Sprint sprint = SprintBuilder{}.forTask(someTask);

    initializer.taskWriter->save(someTask);
    initializer.sprintWriter->save(sprint);

    initializer.taskReader->unfinishedTasks(
        [&initializer, &expectedTask](const ITaskStorageReader::Items& items) {
            EXPECT_EQ(1, items.size());
            EXPECT_EQ(expectedTask, items.front());
            initializer.quit();
            FAIL();
        });
    initializer.runEventLoop();
}
