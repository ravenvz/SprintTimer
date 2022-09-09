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
#include "api/handlers/ReadTaskTreeHandler.h"
#include "api/dtos/TaskMapper.h"
#include "api/dtos/TaskTreeMapper.h"
#include "api/dtos/TaskTypeMapper.h"
#include "core/SprintTimerException.h"

namespace {

// inline sprint_timer::api::TaskTreeDTO buildSampleTree()
// {
//     using sprint_timer::api::TaskDTO;
//     using sprint_timer::api::TaskNodeDTO;
//     using sprint_timer::api::TaskTypeDTO;
//     using namespace dw;
//
//     using Tags = std::vector<std::string>;
//
//     TaskNodeDTO folder1{TaskDTO{"f1",
//                                 Tags{},
//                                 "folder1",
//                                 0,
//                                 0,
//                                 false,
//                                 dw::current_date_time_local()},
//                         TaskTypeDTO::Folder,
//                         std::nullopt,
//                         std::nullopt,
//                         std::string{}};
//     TaskNodeDTO folder2{TaskDTO{"f2",
//                                 Tags{},
//                                 "folder2",
//                                 0,
//                                 0,
//                                 false,
//                                 dw::current_date_time_local()},
//                         TaskTypeDTO::Folder,
//                         std::nullopt,
//                         std::nullopt,
//                         std::string{}};
//     TaskNodeDTO project1{TaskDTO{"p1",
//                                  Tags{},
//                                  "project1",
//                                  0,
//                                  0,
//                                  false,
//                                  dw::current_date_time_local()},
//                          TaskTypeDTO::Project,
//                          std::nullopt,
//                          std::nullopt,
//                          std::string{}};
//     TaskNodeDTO project2{TaskDTO{"p2",
//                                  Tags{},
//                                  "project2",
//                                  0,
//                                  0,
//                                  false,
//                                  dw::current_date_time_local()},
//                          TaskTypeDTO::Project,
//                          std::nullopt,
//                          std::nullopt,
//                          std::string{}};
//     TaskNodeDTO project3{TaskDTO{"p3",
//                                  Tags{},
//                                  "project3",
//                                  0,
//                                  0,
//                                  false,
//                                  dw::current_date_time_local()},
//                          TaskTypeDTO::Project,
//                          std::nullopt,
//                          std::nullopt,
//                          std::string{}};
//     TaskNodeDTO folder3{TaskDTO{"f3",
//                                 Tags{},
//                                 "folder3",
//                                 0,
//                                 0,
//                                 false,
//                                 dw::current_date_time_local()},
//                         TaskTypeDTO::Folder,
//                         std::nullopt,
//                         std::nullopt,
//                         std::string{}};
//     TaskNodeDTO task1{TaskDTO{"t1",
//                               Tags{"Tag1"},
//                               "task1",
//                               4,
//                               0,
//                               false,
//                               dw::current_date_time_local()},
//                       TaskTypeDTO::Regular,
//                       std::nullopt,
//                       std::nullopt,
//                       std::string{"Some notes for task 1"}};
//     TaskNodeDTO project4{TaskDTO{"p4",
//                                  Tags{},
//                                  "project4",
//                                  0,
//                                  0,
//                                  false,
//                                  dw::current_date_time_local()},
//                          TaskTypeDTO::Project,
//                          std::nullopt,
//                          std::nullopt,
//                          std::string{"Some notes for project4 1"}};
//     TaskNodeDTO task2{
//         TaskDTO{"t2",
//                 Tags{"Tag2"},
//                 "task2",
//                 3,
//                 0,
//                 false,
//                 dw::current_date_time_local()},
//         TaskTypeDTO::Regular,
//         DateTime{Date{Year{2022}, Month{5}, Day{24}},
//         std::chrono::hours{17}}, DateTime{Date{Year{2022}, Month{5},
//         Day{24}},
//                  std::chrono::hours{16} + std::chrono::minutes{45}},
//         std::string{"Some notes for task 2"}};
//     TaskNodeDTO task3{TaskDTO{"t3",
//                               Tags{"Tag3"},
//                               "task3",
//                               4,
//                               0,
//                               false,
//                               dw::current_date_time_local()},
//                       TaskTypeDTO::Regular,
//                       std::nullopt,
//                       std::nullopt,
//                       std::string{"Some notes for task 3"}};
//     TaskNodeDTO task4{TaskDTO{"t4",
//                               Tags{"Tag4"},
//                               "task4",
//                               4,
//                               0,
//                               false,
//                               dw::current_date_time_local()},
//                       TaskTypeDTO::Regular,
//                       std::nullopt,
//                       std::nullopt,
//                       std::string{}};
//     TaskNodeDTO task5{TaskDTO{"t5",
//                               Tags{"Tag5"},
//                               "task5",
//                               4,
//                               0,
//                               false,
//                               dw::current_date_time_local()},
//                       TaskTypeDTO::Regular,
//                       std::nullopt,
//                       std::nullopt,
//                       std::string{"Some notes for task 5"}};
//     TaskNodeDTO task6{TaskDTO{"t6",
//                               Tags{"Tag6"},
//                               "task6",
//                               5,
//                               5,
//                               false,
//                               dw::current_date_time_local()},
//                       TaskTypeDTO::Regular,
//                       std::nullopt,
//                       std::nullopt,
//                       std::string{"Some notes for task 6"}};
//     TaskNodeDTO recurringTask1{TaskDTO{"r1",
//                                        Tags{},
//                                        "recurringTask1",
//                                        2,
//                                        0,
//                                        false,
//                                        dw::current_date_time_local()},
//                                TaskTypeDTO::Recurring,
//                                std::nullopt,
//                                std::nullopt,
//                                std::string{"Some notes for recurringTask1"}};
//     TaskNodeDTO task7{TaskDTO{"t7",
//                               Tags{"Tag7"},
//                               "task7",
//                               4,
//                               0,
//                               false,
//                               dw::current_date_time_local()},
//                       TaskTypeDTO::Regular,
//                       std::nullopt,
//                       std::nullopt,
//                       std::string{"Some notes for task 7"}};
//
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
//     sprint_timer::api::TaskTreeDTO tree;
//     tree.addChild("f1", folder1, std::nullopt);
//     tree.addChild("f2", folder2, "f1");
//     tree.addChild("p1", project1, "f2");
//     tree.addChild("p2", project2, "p1");
//     tree.addChild("p3", project3, "p1");
//     tree.addChild("f3", folder3, "p3");
//     tree.addChild("t1", task1, "f3");
//     tree.addChild("p4", project4, std::nullopt);
//     tree.addChild("t2", task2, "p4");
//     tree.addChild("t3", task3, "t2");
//     tree.addChild("t4", task4, "t2");
//     tree.addChild("t5", task5, "t4");
//     tree.addChild("r1", recurringTask1, "t4");
//     tree.addChild("t6", task6, "p4");
//     tree.addChild("t7", task7, std::nullopt);
//     return tree;
// }

} // namespace

namespace sprint_timer::api {

ReadTaskTreeHandler::ReadTaskTreeHandler(
    TaskStorageReader& taskStorageReader_,
    TaskTreeMetadataReader& taskTreeMetadataReader_)
    : taskStorageReader{taskStorageReader_}
    , taskTreeMetadataReader{taskTreeMetadataReader_}
{
}

ReadTaskTreeQuery::Result
ReadTaskTreeHandler::handle(const ReadTaskTreeQuery& /*query*/)
{
    const auto metaTree = taskTreeMetadataReader.readTree();

    std::vector<std::string> uuids;
    std::ranges::copy(metaTree.keys(), std::back_inserter(uuids));
    auto tasks = taskStorageReader.findMatching(uuids);
    std::unordered_map<std::string, Task> taskMap;
    for (auto& task : tasks) {
        taskMap.insert({task.uuid(), std::move(task)});
    }

    auto combine = [&](const auto& payload) {
        auto it = taskMap.find(payload.uuid);
        if (it == taskMap.cend()) {
            throw SprintTimerException{"Error reading task tree"};
        }
        return TaskNodeDTO{makeDTO(it->second), makeDTO(payload.taskType)};
    };

    return metaTree.mapped<TaskNodeDTO>(combine);
}

} // namespace sprint_timer::api
