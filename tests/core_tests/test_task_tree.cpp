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

namespace sprint_timer {

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskNode& taskNode)
{
    os << "TaskNode{" << taskNode.task << ", type: " << taskNode.type
       << ", dueTime: ";
    if (auto dt = taskNode.dueTime; dt) {
        os << *dt;
    }
    else {
        os << "null";
    }
    os << ", reminder: ";
    if (auto rmd = taskNode.reminder; rmd) {
        os << *rmd;
    }
    else {
        os << "null";
    }
    os << "}";
    return os;
}

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
    case Recurring:
        os << "Recurring";
        break;
    }
    return os;
}

} // namespace sprint_timer

class TaskTreeFixture : public ::testing::Test {
public:
    TaskNode folder1{entities::Task{"folder1",
                                    0,
                                    0,
                                    "f1",
                                    std::list<entities::Tag>{},
                                    false,
                                    dw::current_date_time()},
                     TaskType::Folder,
                     std::nullopt,
                     std::nullopt,
                     std::string{}};
    TaskNode folder2{entities::Task{"folder2",
                                    0,
                                    0,
                                    "f2",
                                    std::list<entities::Tag>{},
                                    false,
                                    dw::current_date_time()},
                     TaskType::Folder,
                     std::nullopt,
                     std::nullopt,
                     std::string{}};
    TaskNode project1{entities::Task{"project1",
                                     0,
                                     0,
                                     "p1",
                                     std::list<entities::Tag>{},
                                     false,
                                     dw::current_date_time()},
                      TaskType::Project,
                      std::nullopt,
                      std::nullopt,
                      std::string{}};
    TaskNode project2{entities::Task{"project2",
                                     0,
                                     0,
                                     "p2",
                                     std::list<entities::Tag>{},
                                     false,
                                     dw::current_date_time()},
                      TaskType::Project,
                      std::nullopt,
                      std::nullopt,
                      std::string{}};
    TaskNode project3{entities::Task{"project3",
                                     0,
                                     0,
                                     "p3",
                                     std::list<entities::Tag>{},
                                     false,
                                     dw::current_date_time()},
                      TaskType::Project,
                      std::nullopt,
                      std::nullopt,
                      std::string{}};
    TaskNode folder3{entities::Task{"folder3",
                                    0,
                                    0,
                                    "f3",
                                    std::list<entities::Tag>{},
                                    false,
                                    dw::current_date_time()},
                     TaskType::Folder,
                     std::nullopt,
                     std::nullopt,
                     std::string{}};
    TaskNode task1{
        entities::Task{"task1",
                       4,
                       0,
                       "t1",
                       std::list<entities::Tag>{entities::Tag{"Tag1"}},
                       false,
                       dw::current_date_time()},
        TaskType::Regular,
        std::nullopt,
        std::nullopt,
        std::string{"Some notes for task 1"}};
    TaskNode project4{entities::Task{"project4",
                                     4,
                                     0,
                                     "p4",
                                     std::list<entities::Tag>{},
                                     false,
                                     dw::current_date_time()},
                      TaskType::Project,
                      std::nullopt,
                      std::nullopt,
                      std::string{"Some notes for project4 1"}};
    TaskNode task2{
        entities::Task{"task2",
                       3,
                       0,
                       "t2",
                       std::list<entities::Tag>{entities::Tag{"Tag2"}},
                       false,
                       dw::current_date_time()},
        TaskType::Regular,
        DateTime{Date{Year{2022}, Month{5}, Day{24}}, std::chrono::hours{17}},
        DateTime{Date{Year{2022}, Month{5}, Day{24}},
                 std::chrono::hours{16} + std::chrono::minutes{45}},
        std::string{"Some notes for task 2"}};
    TaskNode task3{
        entities::Task{"task3",
                       4,
                       0,
                       "t3",
                       std::list<entities::Tag>{entities::Tag{"Tag3"}},
                       false,
                       dw::current_date_time()},
        TaskType::Regular,
        std::nullopt,
        std::nullopt,
        std::string{"Some notes for task 3"}};
    TaskNode task4{
        entities::Task{"task4",
                       4,
                       0,
                       "t4",
                       std::list<entities::Tag>{entities::Tag{"Tag4"}},
                       false,
                       dw::current_date_time()},
        TaskType::Regular,
        std::nullopt,
        std::nullopt,
        std::string{}};
    TaskNode task5{
        entities::Task{"task5",
                       4,
                       0,
                       "t5",
                       std::list<entities::Tag>{entities::Tag{"Tag5"}},
                       false,
                       dw::current_date_time()},
        TaskType::Regular,
        std::nullopt,
        std::nullopt,
        std::string{"Some notes for task 5"}};
    TaskNode task6{
        entities::Task{"task6",
                       5,
                       5,
                       "t6",
                       std::list<entities::Tag>{entities::Tag{"Tag6"}},
                       true,
                       dw::current_date_time()},
        TaskType::Regular,
        std::nullopt,
        std::nullopt,
        std::string{"Some notes for task 6"}};
    TaskNode recurringTask1{entities::Task{"recurringTask1",
                                           2,
                                           0,
                                           "r1",
                                           std::list<entities::Tag>{},
                                           false,
                                           dw::current_date_time()},
                            TaskType::Recurring,
                            std::nullopt,
                            std::nullopt,
                            std::string{"Some notes for recurringTask1"}};
    TaskNode task7{
        entities::Task{"task7",
                       4,
                       0,
                       "t7",
                       std::list<entities::Tag>{entities::Tag{"Tag7"}},
                       false,
                       dw::current_date_time()},
        TaskType::Regular,
        std::nullopt,
        std::nullopt,
        std::string{"Some notes for task 7"}};

    TaskTree buildSampleTree()
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
        TaskTree tree;
        insertTask(tree, folder1, std::nullopt);
        insertTask(tree, folder2, "f1");
        insertTask(tree, project1, "f2");
        insertTask(tree, project2, "p1");
        insertTask(tree, project3, "p1");
        insertTask(tree, folder3, "p3");
        insertTask(tree, task1, "f3");
        insertTask(tree, project4, std::nullopt);
        insertTask(tree, task2, "p4");
        insertTask(tree, task3, "t2");
        insertTask(tree, task4, "t2");
        insertTask(tree, task5, "t4");
        insertTask(tree, recurringTask1, "t4");
        insertTask(tree, task6, "p4");
        insertTask(tree, task7, std::nullopt);
        return tree;
    }
};

TEST_F(TaskTreeFixture, returns_immediate_tasks)
{
    const TaskTree tree = buildSampleTree();
    std::vector<TaskNode> expected{
        project2, task1, task3, task5, recurringTask1, task6, task7};

    EXPECT_EQ(expected, immediateTasks(tree));
}

TEST_F(TaskTreeFixture, returns_projects_subtrees)
{
    const TaskTree tree = buildSampleTree();
    TaskTree expected;
    insertTask(expected, project1, std::nullopt);
    insertTask(expected, project2, "p1");
    insertTask(expected, project3, "p1");
    insertTask(expected, folder3, "p3");
    insertTask(expected, task1, "f3");
    insertTask(expected, project4, std::nullopt);
    insertTask(expected, task2, "p4");
    insertTask(expected, task3, "t2");
    insertTask(expected, task4, "t2");
    insertTask(expected, task5, "t4");
    insertTask(expected, recurringTask1, "t4");
    insertTask(expected, task6, "p4");

    EXPECT_EQ(expected, projects(tree));
}
