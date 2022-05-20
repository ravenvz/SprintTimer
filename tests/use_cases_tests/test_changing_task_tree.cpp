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
#include "gtest/gtest.h"
#include "core/TaskTree.h"
#include "core/ObservableActionInvoker.h"
#include "core/TaskStorage.h"
#include "core/use_cases/TaskDTO.h"
#include "core/use_cases/create_task/CreateTaskHandler.h"
#include "core/use_cases/request_tags/AllTagsHandler.h"
#include "core/use_cases/request_tasks/ActiveTasksHandler.h"
#include "use_cases_tests/QtStorageInitializer.h"
#include "use_cases_tests/matchers/MatchesTaskIgnoringUuid.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;


namespace sprint_timer {

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskNode& taskNode)
{
    os << "TaskNode{" << taskNode.taskType << ", " << taskNode.name << ", "
       << taskNode.uuid << ", ";
    os << taskNode.progress.actual() << "/" << taskNode.progress.estimated()
       << ", " << taskNode.completed << ", " << taskNode.lastModified << "}";
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

class ChangingTaskTreeFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    QueryHandler<AllTagsQuery>& allTagsHandler{queryComposer.allTagsHandler()};

    TaskNode folder1{"folder1",
                     "f1",
                     GoalProgress{},
                     false,
                     TaskType::Folder,
                     dw::current_date_time()};
    TaskNode folder2{"folder2",
                     "f2",
                     GoalProgress{},
                     false,
                     TaskType::Folder,
                     dw::current_date_time()};
    TaskNode project1{"project1",
                      "p1",
                      GoalProgress{},
                      false,
                      TaskType::Project,
                      dw::current_date_time()};
    TaskNode project2{"project2",
                      "p2",
                      GoalProgress{},
                      false,
                      TaskType::Project,
                      dw::current_date_time()};
    TaskNode project3{"project3",
                      "p3",
                      GoalProgress{},
                      false,
                      TaskType::Project,
                      dw::current_date_time()};
    TaskNode folder3{"folder3",
                     "f3",
                     GoalProgress{},
                     false,
                     TaskType::Folder,
                     dw::current_date_time()};
    TaskNode task1{"task1",
                   "t1",
                   GoalProgress{GoalProgress::Estimated{4}},
                   false,
                   TaskType::Regular,
                   dw::current_date_time()};
    TaskNode project4{"project4",
                      "p4",
                      GoalProgress{},
                      false,
                      TaskType::Project,
                      dw::current_date_time()};
    TaskNode task2{"task2",
                   "t2",
                   GoalProgress{GoalProgress::Estimated{3}},
                   false,
                   TaskType::Regular,
                   dw::current_date_time()};
    TaskNode task3{"task3",
                   "t3",
                   GoalProgress{GoalProgress::Estimated{12}},
                   false,
                   TaskType::Regular,
                   dw::current_date_time()};
    TaskNode task4{"task4",
                   "t4",
                   GoalProgress{GoalProgress::Estimated{10}},
                   false,
                   TaskType::Regular,
                   dw::current_date_time()};
    TaskNode task5{"task5",
                   "t5",
                   GoalProgress{GoalProgress::Estimated{7}},
                   false,
                   TaskType::Regular,
                   dw::current_date_time()};
    TaskNode task6{"task6",
                   "t6",
                   GoalProgress{GoalProgress::Estimated{2}},
                   false,
                   TaskType::Regular,
                   dw::current_date_time()};
    TaskNode recurringTask1{"recurringTask1",
                            "r1",
                            GoalProgress{},
                            false,
                            TaskType::Recurring,
                            dw::current_date_time()};
    TaskNode task7{"task7",
                   "t7",
                   GoalProgress{GoalProgress::Estimated{5}},
                   false,
                   TaskType::Regular,
                   dw::current_date_time()};

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
        tree.addChild(folder1, std::nullopt);
        tree.addChild(folder2, "f1");
        tree.addChild(project1, "f2");
        tree.addChild(project2, "p1");
        tree.addChild(project3, "p1");
        tree.addChild(folder3, "p3");
        tree.addChild(task1, "f3");
        tree.addChild(project4, std::nullopt);
        tree.addChild(task2, "p4");
        tree.addChild(task3, "t2");
        tree.addChild(task4, "t2");
        tree.addChild(task5, "t4");
        tree.addChild(recurringTask1, "t4");
        tree.addChild(task6, "p4");
        tree.addChild(task7, std::nullopt);
        return tree;
    }
};

TEST_F(ChangingTaskTreeFixture, test_name) {
    FAIL();
}
