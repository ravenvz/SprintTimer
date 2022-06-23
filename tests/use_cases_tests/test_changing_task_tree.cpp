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
#include "core/ObservableActionInvoker.h"
#include "core/TaskStorage.h"
#include "core/TaskTree.h"
#include "core/use_cases/TaskTreeMapper.h"
#include "core/use_cases/create_task/CreateTaskHandler.h"
#include "core/use_cases/request_tags/AllTagsHandler.h"
#include "core/use_cases/request_tasks/ActiveTasksHandler.h"
#include "use_cases_tests/QtStorageInitializer.h"
#include "use_cases_tests/matchers/MatchesTaskIgnoringUuid.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;

namespace sprint_timer {

// template <class CharT, class Traits>
// std::basic_ostream<CharT, Traits>&
// operator<<(std::basic_ostream<CharT, Traits>& os, const TaskNode& taskNode)
// {
//     os << "TaskNode{" << taskNode.task << ", type: " << taskNode.type
//        << ", dueTime: ";
//     if (auto dt = taskNode.dueTime; dt) {
//         os << *dt;
//     }
//     else {
//         os << ", null";
//     }
//     os << ", reminder: ";
//     if (auto rmd = taskNode.reminder; rmd) {
//         os << *rmd;
//     }
//     else {
//         os << ", null";
//     }
//     os << "}";
//     return os;
// }

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
    void SetUp() override
    {
        std::filesystem::create_directory(initializer.fileStoragePath);
    }

    void TearDown() override
    {
        std::filesystem::remove_all(initializer.fileStoragePath);
    }

    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    QueryHandler<AllTagsQuery>& allTagsHandler{queryComposer.allTagsHandler()};
    CommandHandler<SaveTaskTreeCommand>& saveTaskTreeHandler{
        commandComposer.saveTaskTreeHandler()};
    QueryHandler<ReadTaskTreeQuery>& readTaskTreeHandler{
        queryComposer.readTaskTreeHandler()};
    TaskNode folder1;
    TaskNode folder2;
    TaskNode project1;
    TaskNode project2;
    TaskNode project3;
    TaskNode folder3;
    TaskNode task1;
    TaskNode project4;
    TaskNode task2;
    TaskNode task3;
    TaskNode task4;
    TaskNode task5;
    TaskNode recurringTask1;
    TaskNode task6;
    TaskNode task7;

    TaskTree buildSampleTree()
    {
        using Tags = std::vector<std::string>;
        createTaskHandler.handle(CreateTaskCommand{"folder1", Tags{}, 0});
        createTaskHandler.handle(CreateTaskCommand{"folder2", Tags{}, 0});
        createTaskHandler.handle(CreateTaskCommand{"project1", Tags{}, 0});
        createTaskHandler.handle(CreateTaskCommand{"project2", Tags{}, 0});
        createTaskHandler.handle(CreateTaskCommand{"project3", Tags{}, 0});
        createTaskHandler.handle(CreateTaskCommand{"folder3", Tags{}, 0});
        createTaskHandler.handle(CreateTaskCommand{"task1", Tags{"Tag1"}, 4});
        createTaskHandler.handle(CreateTaskCommand{"project4", Tags{}, 0});
        createTaskHandler.handle(CreateTaskCommand{"task2", Tags{"Tag2"}, 3});
        createTaskHandler.handle(CreateTaskCommand{"task3", Tags{"Tag3"}, 4});
        createTaskHandler.handle(CreateTaskCommand{"task4", Tags{"Tag4"}, 4});
        createTaskHandler.handle(CreateTaskCommand{"task5", Tags{"Tag5"}, 4});
        createTaskHandler.handle(CreateTaskCommand{"task6", Tags{"Tag6"}, 5});
        createTaskHandler.handle(
            CreateTaskCommand{"recurringTask1", Tags{}, 2});
        createTaskHandler.handle(CreateTaskCommand{"task7", Tags{"Tag7"}, 4});

        std::unordered_map<std::string, std::string> uuid_map;
        uuid_map["f1"] = "0";
        uuid_map["f2"] = "1";
        uuid_map["p1"] = "2";
        uuid_map["p2"] = "3";
        uuid_map["p3"] = "4";
        uuid_map["f3"] = "5";
        uuid_map["t1"] = "6";
        uuid_map["p4"] = "7";
        uuid_map["t2"] = "8";
        uuid_map["t3"] = "9";
        uuid_map["t4"] = "10";
        uuid_map["t5"] = "11";
        uuid_map["t6"] = "12";
        uuid_map["r1"] = "13";
        uuid_map["t7"] = "14";

        folder1 = TaskNode{entities::Task{"folder1",
                                          0,
                                          0,
                                          uuid_map["f1"],
                                          std::list<entities::Tag>{},
                                          false,
                                          dw::current_date_time_local()},
                           TaskType::Folder,
                           std::nullopt,
                           std::nullopt,
                           std::string{}};
        folder2 = TaskNode{entities::Task{"folder2",
                                          0,
                                          0,
                                          uuid_map["f2"],
                                          std::list<entities::Tag>{},
                                          false,
                                          dw::current_date_time_local()},
                           TaskType::Folder,
                           std::nullopt,
                           std::nullopt,
                           std::string{}};
        project1 = TaskNode{entities::Task{"project1",
                                           0,
                                           0,
                                           uuid_map["p1"],
                                           std::list<entities::Tag>{},
                                           false,
                                           dw::current_date_time_local()},
                            TaskType::Project,
                            std::nullopt,
                            std::nullopt,
                            std::string{}};
        project2 = TaskNode{entities::Task{"project2",
                                           0,
                                           0,
                                           uuid_map["p2"],
                                           std::list<entities::Tag>{},
                                           false,
                                           dw::current_date_time_local()},
                            TaskType::Project,
                            std::nullopt,
                            std::nullopt,
                            std::string{}};
        project3 = TaskNode{entities::Task{"project3",
                                           0,
                                           0,
                                           uuid_map["p3"],
                                           std::list<entities::Tag>{},
                                           false,
                                           dw::current_date_time_local()},
                            TaskType::Project,
                            std::nullopt,
                            std::nullopt,
                            std::string{}};
        folder3 = TaskNode{entities::Task{"folder3",
                                          0,
                                          0,
                                          uuid_map["f3"],
                                          std::list<entities::Tag>{},
                                          false,
                                          dw::current_date_time_local()},
                           TaskType::Folder,
                           std::nullopt,
                           std::nullopt,
                           std::string{}};
        task1 = TaskNode{
            entities::Task{"task1",
                           4,
                           0,
                           uuid_map["t1"],
                           std::list<entities::Tag>{entities::Tag{"Tag1"}},
                           false,
                           dw::current_date_time_local()},
            TaskType::Regular,
            std::nullopt,
            std::nullopt,
            std::string{"Some notes for task 1"}};
        project4 = TaskNode{entities::Task{"project4",
                                           0,
                                           0,
                                           uuid_map["p4"],
                                           std::list<entities::Tag>{},
                                           false,
                                           dw::current_date_time_local()},
                            TaskType::Project,
                            std::nullopt,
                            std::nullopt,
                            std::string{"Some notes for project4 1"}};
        task2 = TaskNode{
            entities::Task{"task2",
                           3,
                           0,
                           uuid_map["t2"],
                           std::list<entities::Tag>{entities::Tag{"Tag2"}},
                           false,
                           dw::current_date_time_local()},
            TaskType::Regular,
            DateTime{Date{Year{2022}, Month{5}, Day{24}},
                     std::chrono::hours{17}},
            DateTime{Date{Year{2022}, Month{5}, Day{24}},
                     std::chrono::hours{16} + std::chrono::minutes{45}},
            std::string{"Some notes for task 2"}};
        task3 = TaskNode{
            entities::Task{"task3",
                           4,
                           0,
                           uuid_map["t3"],
                           std::list<entities::Tag>{entities::Tag{"Tag3"}},
                           false,
                           dw::current_date_time_local()},
            TaskType::Regular,
            std::nullopt,
            std::nullopt,
            std::string{"Some notes for task 3"}};
        task4 = TaskNode{
            entities::Task{"task4",
                           4,
                           0,
                           uuid_map["t4"],
                           std::list<entities::Tag>{entities::Tag{"Tag4"}},
                           false,
                           dw::current_date_time_local()},
            TaskType::Regular,
            std::nullopt,
            std::nullopt,
            std::string{}};
        task5 = TaskNode{
            entities::Task{"task5",
                           4,
                           0,
                           uuid_map["t5"],
                           std::list<entities::Tag>{entities::Tag{"Tag5"}},
                           false,
                           dw::current_date_time_local()},
            TaskType::Regular,
            std::nullopt,
            std::nullopt,
            std::string{"Some notes for task 5"}};
        task6 = TaskNode{
            entities::Task{"task6",
                           5,
                           5,
                           uuid_map["t6"],
                           std::list<entities::Tag>{entities::Tag{"Tag6"}},
                           false,
                           dw::current_date_time_local()},
            TaskType::Regular,
            std::nullopt,
            std::nullopt,
            std::string{"Some notes for task 6"}};
        recurringTask1 = TaskNode{entities::Task{"recurringTask1",
                                                 2,
                                                 0,
                                                 uuid_map["r1"],
                                                 std::list<entities::Tag>{},
                                                 false,
                                                 dw::current_date_time_local()},
                                  TaskType::Recurring,
                                  std::nullopt,
                                  std::nullopt,
                                  std::string{"Some notes for recurringTask1"}};
        task7 = TaskNode{
            entities::Task{"task7",
                           4,
                           0,
                           uuid_map["t7"],
                           std::list<entities::Tag>{entities::Tag{"Tag7"}},
                           false,
                           dw::current_date_time_local()},
            TaskType::Regular,
            std::nullopt,
            std::nullopt,
            std::string{"Some notes for task 7"}};

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
        insertTask(tree, folder2, uuid_map["f1"]);
        insertTask(tree, project1, uuid_map["f2"]);
        insertTask(tree, project2, uuid_map["p1"]);
        insertTask(tree, project3, uuid_map["p1"]);
        insertTask(tree, folder3, uuid_map["p3"]);
        insertTask(tree, task1, uuid_map["f3"]);
        insertTask(tree, project4, std::nullopt);
        insertTask(tree, task2, uuid_map["p4"]);
        insertTask(tree, task3, uuid_map["t2"]);
        insertTask(tree, task4, uuid_map["t2"]);
        insertTask(tree, task5, uuid_map["t4"]);
        insertTask(tree, recurringTask1, uuid_map["t4"]);
        insertTask(tree, task6, uuid_map["p4"]);
        insertTask(tree, task7, std::nullopt);
        return tree;
    }
};

TEST_F(ChangingTaskTreeFixture, saving_and_restoring_tree)
{
    using namespace sprint_timer::use_cases;
    const auto tree = buildSampleTree();
    saveTaskTreeHandler.handle(SaveTaskTreeCommand{makeDTO(tree)});

    const auto actual = readTaskTreeHandler.handle(ReadTaskTreeQuery{});

    EXPECT_EQ(makeDTO(tree), actual);
}
