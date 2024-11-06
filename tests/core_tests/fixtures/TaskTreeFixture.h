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
#ifndef TASKTREEFIXTURE_H_CRQNFDYB
#define TASKTREEFIXTURE_H_CRQNFDYB

#include "core/Task.h"

using namespace sprint_timer;
using namespace std::chrono_literals;
using namespace dw;

namespace fixtures {

struct TaskTreeFixture {
    explicit TaskTreeFixture(dw::DateTime anchorTime_)
        : anchorTime{anchorTime_}
    {
        make_tree();
    }

    auto makeDefaultTimeFrame() const -> TaskTimeframe
    {
        return TaskTimeframe{anchorTime};
    }

    DateTime anchorTime;

    Task folder1{"folder1",
                 0,
                 {},
                 "f1",
                 {},
                 false,
                 anchorTime - 3h,
                 TaskType::Folder,
                 std::nullopt,
                 makeDefaultTimeFrame()};

    Task folder2{"folder2",
                 0,
                 {},
                 "f2",
                 {},
                 false,
                 anchorTime - 2h,
                 TaskType::Folder,
                 std::nullopt,
                 makeDefaultTimeFrame()};

    Task project1{"project1",
                  0,
                  {},
                  "p1",
                  {},
                  false,
                  anchorTime - Days{7},
                  TaskType::Project,
                  std::nullopt,
                  makeDefaultTimeFrame()};

    Task project2{"project2",
                  0,
                  {},
                  "p2",
                  {},
                  false,
                  anchorTime - Days{1} - 1h,
                  TaskType::Project,
                  std::nullopt,
                  makeDefaultTimeFrame()};

    Task project3{"project3",
                  0,
                  {},
                  "p3",
                  {},
                  false,
                  anchorTime - Days{30},
                  TaskType::Project,
                  std::nullopt,
                  makeDefaultTimeFrame()};

    Task folder3{"folder3",
                 0,
                 {},
                 "f3",
                 {},
                 false,
                 anchorTime - 1h,
                 TaskType::Folder,
                 std::nullopt,
                 makeDefaultTimeFrame()};

    Task task1{"task1",
               4,
               {},
               "t1",
               {Tag{"Tag1"}},
               false,
               anchorTime - Days{15},
               TaskType::Regular,
               Note{"Some notes for task 1"},
               makeDefaultTimeFrame()};

    Task project4{"project4",
                  0,
                  {},
                  "p4",
                  {Tag{"Tag2"}},
                  false,
                  anchorTime,
                  TaskType::Project,
                  Note{"Some notes for project 4"},
                  makeDefaultTimeFrame()};

    Task task2{"task2",
               3,
               {},
               "t2",
               {Tag{"Tag2"}},
               false,
               anchorTime - Days{5},
               TaskType::Regular,
               Note{"Some notes for task 2"},
               TaskTimeframe{DateTime{anchorTime - Years{2}},
                             DateTime{anchorTime - Years{2} + Weeks{3}}}};

    Task task3{"task3",
               4,
               {},
               "t3",
               {Tag{"Tag3"}},
               false,
               anchorTime - Days{2},
               TaskType::Regular,
               Note{"Some notes for task 3"},
               makeDefaultTimeFrame()};

    Task task4{"task4",
               4,
               {},
               "t4",
               {Tag{"Tag4"}},
               false,
               anchorTime - 3min,
               TaskType::Regular,
               Note{"Some notes for task 3"},
               makeDefaultTimeFrame()};

    Task task5{"task5",
               4,
               {},
               "t4",
               {Tag{"Tag2"}, Tag{"Tag5"}},
               false,
               anchorTime - Days{1},
               TaskType::Regular,
               Note{"Some notes for task 5"},
               TaskTimeframe{DateTime{anchorTime + Days{20}},
                             DateTime{anchorTime + Days{30}}}};

    Task task6{"task6",
               5,
               {DateTimeRange{anchorTime, anchorTime + 25min},
                DateTimeRange{anchorTime, anchorTime + 25min},
                DateTimeRange{anchorTime, anchorTime + 25min},
                DateTimeRange{anchorTime, anchorTime + 25min},
                DateTimeRange{anchorTime, anchorTime + 25min}},
               "t6",
               {Tag{"Tag6"}},
               true,
               anchorTime - Days{32},
               TaskType::Regular,
               Note{"Some notes for task 6"},
               makeDefaultTimeFrame()};

    Task task7{"task7",
               4,
               {},
               "t7",
               {Tag{"Tag7"}},
               false,
               anchorTime - Days{6},
               TaskType::Regular,
               Note{"Some notes for task 7"},
               makeDefaultTimeFrame()};

    TaskTree tree;

    auto make_tree() -> void
    {
        /*
         * folder1
         *    folder2
         *       project1
         *          project2
         *          project3
         *             folder3
         *                task1
         * project4
         *    task2
         *       task3
         *       task4
         *          task5
         *          recurringTask1
         *    task6
         * task7
         */
        auto uuid_projection = [](const auto& node) { return node.uuid(); };
        tree.insert(tree.end(), folder1);
        tree.insert(std::ranges::find(tree, folder1.uuid(), uuid_projection),
                    folder2);
        tree.insert(std::ranges::find(tree, folder2.uuid(), uuid_projection),
                    project1);
        tree.insert(std::ranges::find(tree, project1.uuid(), uuid_projection),
                    project2);
        tree.insert(std::ranges::find(tree, project1.uuid(), uuid_projection),
                    project3);
        tree.insert(std::ranges::find(tree, project3.uuid(), uuid_projection),
                    folder3);
        tree.insert(std::ranges::find(tree, folder3.uuid(), uuid_projection),
                    task1);
        tree.insert(tree.end(), project4);
        tree.insert(std::ranges::find(tree, project4.uuid(), uuid_projection),
                    task2);
        tree.insert(std::ranges::find(tree, task2.uuid(), uuid_projection),
                    task3);
        tree.insert(std::ranges::find(tree, task2.uuid(), uuid_projection),
                    task4);
        tree.insert(std::ranges::find(tree, task4.uuid(), uuid_projection),
                    task5);
        tree.insert(std::ranges::find(tree, task4.uuid(), uuid_projection),
                    recurringTask1);
        tree.insert(std::ranges::find(tree, project4.uuid(), uuid_projection),
                    task6);
        tree.insert(tree.end(), task7);
    }
};

} // namespace fixtures

#endif /* end of include guard: TASKTREEFIXTURE_H_CRQNFDYB */

