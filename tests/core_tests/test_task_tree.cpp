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
#include "core/TaskTree.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace dw;
using namespace std::chrono_literals;

namespace sprint_timer {

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskType& taskType)
{
    using enum TaskType;
    os << "TaskType::";
    switch (taskType) {
    case Regular:
        os << "Regular";
        break;
    case Project:
        os << "Project";
        break;
    case Folder:
        os << "Folder";
        break;
    }
    return os;
}

} // namespace sprint_timer

class TaskTreeFixture : public ::testing::Test {
public:
    Task folder1{"folder1",
                 0,
                 {},
                 "f1",
                 {},
                 false,
                 dw::current_date_time(),
                 TaskType::Folder,
                 std::nullopt,
                 TaskTimeframe{}};

    Task folder2{"folder2",
                 0,
                 {},
                 "f2",
                 {},
                 false,
                 dw::current_date_time(),
                 TaskType::Folder,
                 std::nullopt,
                 TaskTimeframe{}};

    Task project1{"project1",
                  0,
                  {},
                  "p1",
                  {},
                  false,
                  dw::current_date_time(),
                  TaskType::Project,
                  std::nullopt,
                  TaskTimeframe{}};

    Task project2{"project2",
                  0,
                  {},
                  "p2",
                  {},
                  false,
                  dw::current_date_time(),
                  TaskType::Project,
                  std::nullopt,
                  TaskTimeframe{}};

    Task project3{"project3",
                  0,
                  {},
                  "p3",
                  {},
                  false,
                  dw::current_date_time(),
                  TaskType::Project,
                  std::nullopt,
                  TaskTimeframe{}};

    Task folder3{"folder3",
                 0,
                 {},
                 "f3",
                 {},
                 false,
                 dw::current_date_time(),
                 TaskType::Folder,
                 std::nullopt,
                 TaskTimeframe{}};

    Task task1{"task1",
               4,
               {},
               "t1",
               {Tag{"Tag1"}},
               false,
               dw::current_date_time(),
               TaskType::Regular,
               Note{"Some notes for task 1"},
               TaskTimeframe{}};

    Task project4{"project4",
                  4,
                  {},
                  "p4",
                  {},
                  false,
                  dw::current_date_time(),
                  TaskType::Project,
                  Note{"Some notes for project4 1"},
                  TaskTimeframe{}};

    Task task2{"task2",
               3,
               {},
               "t2",
               {Tag{"Tag2"}},
               false,
               dw::current_date_time(),
               TaskType::Regular,
               Note{"Some notes for task 2"},
               TaskTimeframe{
                   DateTime{Date{Year{2022}, Month{5}, Day{24}},
                            std::chrono::hours{17}},
                   DateTime{Date{Year{2022}, Month{5}, Day{30}},
                            std::chrono::hours{16} + std::chrono::minutes{45}},
                   std::nullopt,
                   std::nullopt}};

    Task task3{"task3",
               4,
               {},
               "t3",
               {Tag{"Tag3"}},
               false,
               dw::current_date_time(),
               TaskType::Regular,
               Note{"Some notes for task 3"},
               TaskTimeframe{}};

    Task task4{"task4",
               4,
               {},
               "t4",
               {Tag{"Tag4"}},
               false,
               dw::current_date_time(),
               TaskType::Regular,
               std::nullopt,
               TaskTimeframe{}};

    Task task5{"task5",
               4,
               {},
               "t5",
               {Tag{"Tag5"}},
               false,
               dw::current_date_time(),
               TaskType::Regular,
               Note{"Some notes for task 5"},
               TaskTimeframe{}};

    Task task6{"task6",
               5,
               {Sprint{current_date_time(), 25min},
                Sprint{current_date_time(), 25min},
                Sprint{current_date_time(), 25min},
                Sprint{current_date_time(), 25min},
                Sprint{current_date_time(), 25min}},
               "t6",
               {Tag{"Tag6"}},
               true,
               dw::current_date_time(),
               TaskType::Regular,
               Note{"Some notes for task 6"},
               TaskTimeframe{}};

    Task recurringTask1{"recurringTask1",
                        2,
                        {},
                        "r1",
                        {},
                        false,
                        dw::current_date_time(),
                        TaskType::Regular,
                        Note{"Some notes for recurringTask1"},
                        TaskTimeframe{}};

    Task task7{"task7",
               4,
               {},
               "t7",
               {Tag{"Tag7"}},
               false,
               dw::current_date_time(),
               TaskType::Regular,
               Note{"Some notes for task 7"},
               TaskTimeframe{}};

    // [[nodiscard]] auto buildSampleTree() const -> TaskTree
    // {
    //     #<{(|
    //      * folder1
    //      *    folder2
    //      *       project1
    //      *          project2
    //      *          project3
    //      *             folder3
    //      *                task1
    //      * project4
    //      *    task2
    //      *       task3
    //      *       task4
    //      *          task5
    //      *          recurringTask1
    //      *    task6
    //      * task7
    //      |)}>#
    //     TaskTree tree;
    //     insertTask(tree, folder1, std::nullopt);
    //     insertTask(tree, folder2, "f1");
    //     insertTask(tree, project1, "f2");
    //     insertTask(tree, project2, "p1");
    //     insertTask(tree, project3, "p1");
    //     insertTask(tree, folder3, "p3");
    //     insertTask(tree, task1, "f3");
    //     insertTask(tree, project4, std::nullopt);
    //     insertTask(tree, task2, "p4");
    //     insertTask(tree, task3, "t2");
    //     insertTask(tree, task4, "t2");
    //     insertTask(tree, task5, "t4");
    //     insertTask(tree, recurringTask1, "t4");
    //     insertTask(tree, task6, "p4");
    //     insertTask(tree, task7, std::nullopt);
    //     return tree;
    // }
};

TEST_F(TaskTreeFixture, returns_immediate_tasks)
{
    FAIL();
    // const TaskTree tree = buildSampleTree();
    // std::vector<Task> expected{
    //     project2, task1, task3, task5, recurringTask1, task6, task7};
    //
    // EXPECT_EQ(expected, immediateTasks(tree));
}

TEST_F(TaskTreeFixture, returns_projects_subtrees)
{
    FAIL();

    // const TaskTree tree = buildSampleTree();
    // TaskTree expected;
    // insertTask(expected, project1, std::nullopt);
    // insertTask(expected, project2, "p1");
    // insertTask(expected, project3, "p1");
    // insertTask(expected, folder3, "p3");
    // insertTask(expected, task1, "f3");
    // insertTask(expected, project4, std::nullopt);
    // insertTask(expected, task2, "p4");
    // insertTask(expected, task3, "t2");
    // insertTask(expected, task4, "t2");
    // insertTask(expected, task5, "t4");
    // insertTask(expected, recurringTask1, "t4");
    // insertTask(expected, task6, "p4");
    //
    // EXPECT_EQ(expected, projects(tree));
}
