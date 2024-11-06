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
#include "api/DefaultDateTimeProvider.h"
#include "api/dtos/TaskTreeMapper.h"
#include "api/requests/CreateTaskCommand.h"
#include "api/requests/DeleteTaskCommand.h"
#include "api/requests/EditTaskCommand.h"
#include "api/requests/ReadTaskTreeQuery.h"
#include "api/requests/SaveTaskTreeCommand.h"
#include "api/requests/ToggleTaskCompletedCommand.h"
#include "common_utils/ConfigurableDateTimeProvider.h"
#include "common_utils/TaskDtoTreeFixtures.h"
#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"
#include "qt_gui/presentation/PlannerPresenter.h"
#include "qt_gui/presentation/PredefinedTaskViewFilters.h"
#include "gmock/gmock.h"

namespace {

using sprint_timer::api::TaskTreeDTO;
using sprint_timer::ui::contracts::PlannerContract::Item;
using sprint_timer::ui::contracts::PlannerContract::PlannerItem;
using sprint_timer::ui::contracts::PlannerContract::PlannerTree;

constexpr std::string_view defaultBackground{"defaultBackground"};
constexpr std::string_view textColor{"textColor"};
constexpr std::string_view contrastColor{"contrastColor"};
constexpr std::string_view normalWorkColor{"normalWorkColor"};
constexpr std::string_view doneWorkColor{"doneWorkColor"};
constexpr std::string_view overworkWorkColor{"overworkWorkColor"};
constexpr std::string_view dueNotSoonColor{"dueNotSoonColor"};
constexpr std::string_view dueNotSoonTextColor{"dueNotSoonTextColor"};
constexpr std::string_view dueSoonColor{"dueSoonColor"};
constexpr std::string_view dueTodayColor{"dueTodayColor"};
constexpr std::string_view dueOverdueColor{"dueOverdueColor"};
constexpr std::string_view tagColor{"tagColor"};

using Items = std::vector<PlannerItem>;
using MappedItems = std::vector<std::pair<PlannerItem, Items>>;

auto get_subchildren(const PlannerTree& tree) -> MappedItems
{
    auto map_iterator_to_children = [&](auto it) {
        return std::pair{*it, tree.children(it) | std::ranges::to<Items>()};
    };
    return tree.children_iterators(tree.end()) |
           std::views::transform(map_iterator_to_children) |
           std::ranges::to<MappedItems>();
};

} // namespace

using ::testing::_;
using ::testing::AllOf;
using ::testing::Field;
using ::testing::NiceMock;
using ::testing::Pair;
using ::testing::ResultOf;
using ::testing::Return;
using namespace dw;
using namespace sprint_timer::api;
using namespace std::chrono_literals;
using sprint_timer::ui::PlannerColors;
using sprint_timer::ui::contracts::PlannerContract::PlannerItem;
using sprint_timer::ui::contracts::PlannerContract::PlannerTree;

class PlannerWindowMock
    : public sprint_timer::ui::contracts::PlannerContract::View {
public:
    MOCK_METHOD(void, displayPlanner, ((const PlannerTree&)), (override));
};

class StubDateTimeProvider : public sprint_timer::api::DateTimeProvider {
public:
    StubDateTimeProvider(dw::DateTime anchorTime_)
        : anchorTime{anchorTime_}
    {
    }

private:
    auto dateNowImpl() const -> dw::Date override { return anchorTime.date(); }

    auto dateTimeNowImpl() const -> dw::DateTime override { return anchorTime; }

    auto dateLocalNowImpl() const -> dw::Date override
    {
        return anchorTime.date();
    }

    auto dateTimeLocalNowImpl() const -> dw::DateTime override
    {
        return anchorTime;
    }

private:
    dw::DateTime anchorTime;
};

class PlannerPresenterFixture : public ::testing::Test {
public:
    NiceMock<PlannerWindowMock> view;
    NiceMock<mocks::QueryHandlerMock<sprint_timer::api::ReadTaskTreeQuery>>
        readPlannerHandler;
    NiceMock<mocks::CommandHandlerMock<sprint_timer::api::SaveTaskTreeCommand>>
        savePlannerHandler;
    NiceMock<mocks::CommandHandlerMock<sprint_timer::api::DeleteTaskCommand>>
        deleteTaskHandler;
    NiceMock<mocks::CommandHandlerMock<sprint_timer::api::EditTaskCommand>>
        editTaskHandler;
    NiceMock<mocks::CommandHandlerMock<
        sprint_timer::api::ToggleTaskCompletedCommand>>
        toggleTaskHandler;
    DateTime anchorTime{DateTime{Date{Year{2022}, Month{9}, Day{21}}} + 10h};
    ConfigurableDateTimeProvider dateTimeProvider{
        std::make_unique<DefaultDateTimeProvider>(), anchorTime};
    // std::unique_ptr<DefaultDateTimeProvider> dateTimeProvider =
    // std::make_unique<DefaultDateTimeProvider>();
    sprint_timer::ui::AddTaskContext addTaskContext;
    sprint_timer::ui::EditTaskContext editTaskContext;
    PlannerColors plannerColors{textColor,
                                contrastColor,
                                defaultBackground,
                                normalWorkColor,
                                doneWorkColor,
                                overworkWorkColor,
                                dueNotSoonColor,
                                dueSoonColor,
                                dueTodayColor,
                                dueOverdueColor,
                                tagColor};
    sprint_timer::ui::TaskTreeFilter taskTreeFilter{
        sprint_timer::ui::makeTaskViewFilters(dateTimeProvider)};
    sprint_timer::ui::PlannerPresenter sut{plannerColors,
                                           taskTreeFilter,
                                           readPlannerHandler,
                                           savePlannerHandler,
                                           deleteTaskHandler,
                                           editTaskHandler,
                                           toggleTaskHandler,
                                           addTaskContext,
                                           editTaskContext,
                                           dateTimeProvider};
    fixtures::TaskDtoTreeFixture taskDtoTreeFixture{anchorTime};

    [[nodiscard]] auto buildSampleTree() const -> TaskTreeDTO
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
        return taskDtoTreeFixture.tree;
    }

    auto setupSampleTree()
    {
        auto taskTree = buildSampleTree();
        ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
            .WillByDefault(Return(taskTree));
    }
};

TEST_F(PlannerPresenterFixture, does_nothing_when_view_is_not_attached)
{
    EXPECT_CALL(view, displayPlanner(_)).Times(0);

    sut.updateView();
}

TEST_F(PlannerPresenterFixture, updates_view_when_view_is_attached)
{
    auto matches_tree = [](const PlannerTree& tree) {
        return tree == PlannerTree{};
    };
    ON_CALL(readPlannerHandler, handle(sprint_timer::api::ReadTaskTreeQuery{}))
        .WillByDefault(
            Return(::testing::ByMove(sprint_timer::api::TaskTreeDTO{})));

    EXPECT_CALL(view, displayPlanner(::testing::Truly(matches_tree)));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, updates_view)
{
    TaskTreeDTO taskTree;
    taskTree.insert(
        taskTree.end(),
        TaskDTO{"1",
                {"Tag9", "Tag2"},
                "Some root task",
                2,
                {},
                false,
                anchorTime,
                NoteDTO{"Just some text note"},
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    PlannerTree plannerTree;
    plannerTree.insert(
        plannerTree.end(),
        PlannerItem{"1",
                    {"Some root task", textColor, defaultBackground},
                    {"Tag9, Tag2", tagColor, defaultBackground},
                    {"0/2", textColor, defaultBackground},
                    anchorTime,
                    {"", textColor, defaultBackground},
                    "Just some text note",
                    {"", textColor, defaultBackground},
                    false,
                    false,
                    TaskTypeDTO::Regular});
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_task_without_time_frame)
{
    TaskTreeDTO taskTree;
    taskTree.insert(
        taskTree.end(),
        TaskDTO{"1",
                {"Tag9", "Tag2"},
                "Some root task",
                2,
                {},
                false,
                anchorTime,
                NoteDTO{"Just some text note"},
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    PlannerTree plannerTree;
    plannerTree.insert(
        plannerTree.end(),
        PlannerItem{"1",
                    {"Some root task", textColor, defaultBackground},
                    {"Tag9, Tag2", tagColor, defaultBackground},
                    {"0/2", textColor, defaultBackground},
                    anchorTime,
                    {"", textColor, defaultBackground},
                    "Just some text note",
                    {"", textColor, defaultBackground},
                    false,
                    false,
                    TaskTypeDTO::Regular});
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_task_that_is_due_soon)
{
    TaskTreeDTO taskTree;
    taskTree.insert(taskTree.end(),
                    TaskDTO{"2",
                            {"Tag1", "Tag2"},
                            "Some task that is due soon",
                            5,
                            {},
                            false,
                            anchorTime,
                            NoteDTO{"Just some text note"},
                            TaskTimeframeDTO{anchorTime - Days{2},
                                             anchorTime + Days{3},
                                             std::nullopt,
                                             std::nullopt},
                            TaskTypeDTO::Regular});
    PlannerTree plannerTree;
    plannerTree.insert(
        plannerTree.end(),
        PlannerItem{
            "2",
            {"Some task that is due soon", textColor, defaultBackground},
            {"Tag1, Tag2", tagColor, defaultBackground},
            {"0/5", textColor, defaultBackground},
            anchorTime - Days{2},
            {"Saturday", contrastColor, dueSoonColor},
            "Just some text note",
            {"", textColor, defaultBackground},
            false,
            false,
            TaskTypeDTO::Regular});
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_task_with_reminder)
{
    TaskTreeDTO taskTree;
    taskTree.insert(taskTree.end(),
                    TaskDTO{"3",
                            {"Tag1", "Tag2"},
                            "Some task with reminder",
                            7,
                            {},
                            false,
                            anchorTime,
                            NoteDTO{"Task with reminder note"},
                            TaskTimeframeDTO{anchorTime - Days{2},
                                             anchorTime + Days{5},
                                             anchorTime + Days{5} - 2h,
                                             std::nullopt},
                            TaskTypeDTO::Regular});
    PlannerTree plannerTree;
    plannerTree.insert(
        plannerTree.end(),
        PlannerItem{"3",
                    {"Some task with reminder", textColor, defaultBackground},
                    {"Tag1, Tag2", tagColor, defaultBackground},
                    {"0/7", textColor, defaultBackground},
                    anchorTime - Days{2},
                    {"+5 Days", contrastColor, dueNotSoonColor},
                    "Task with reminder note",
                    {"08:00 26.09.2022", textColor, defaultBackground},
                    false,
                    false,
                    TaskTypeDTO::Regular});
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_project)
{
    TaskTreeDTO taskTree;
    taskTree.insert(taskTree.end(),
                    TaskDTO{"4",
                            {"Tag1", "Tag2"},
                            "Project",
                            7,
                            {},
                            false,
                            anchorTime,
                            NoteDTO{"Task with reminder note"},
                            TaskTimeframeDTO{anchorTime - Days{2},
                                             anchorTime + Days{5},
                                             anchorTime + Days{5} - 2h,
                                             std::nullopt},
                            TaskTypeDTO::Project});
    taskTree.insert(
        std::ranges::find(taskTree, "4", &TaskDTO::uuid),
        TaskDTO{"11",
                {"Tag3"},
                "Sub task 1",
                3,
                {},
                false,
                anchorTime,
                std::nullopt,
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    taskTree.insert(
        std::ranges::find(taskTree, "11", &TaskDTO::uuid),
        TaskDTO{"12",
                {"Tag2"},
                "Sub project 1",
                3,
                {},
                false,
                anchorTime,
                std::nullopt,
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Project});
    taskTree.insert(
        std::ranges::find(taskTree, "12", &TaskDTO::uuid),
        TaskDTO{"13",
                {"Tag7"},
                "Sub task 2",
                3,
                {dw::DateTimeRange{anchorTime, anchorTime},
                 dw::DateTimeRange{anchorTime, anchorTime}},
                true,
                anchorTime,
                std::nullopt,
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    taskTree.insert(
        std::ranges::find(taskTree, "12", &TaskDTO::uuid),
        TaskDTO{"14",
                {"Tag8"},
                "Sub task 3",
                2,
                {dw::DateTimeRange{anchorTime, anchorTime},
                 dw::DateTimeRange{anchorTime, anchorTime}},
                false,
                anchorTime,
                std::nullopt,
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    PlannerTree plannerTree;
    plannerTree.insert(
        plannerTree.end(),
        PlannerItem{"4",
                    {"Project", tagColor, defaultBackground},
                    {"Tag1, Tag2", tagColor, defaultBackground},
                    {"", textColor, defaultBackground},
                    anchorTime - Days{2},
                    {"+5 Days", contrastColor, dueNotSoonColor},
                    "Task with reminder note",
                    {"08:00 26.09.2022", textColor, defaultBackground},
                    false,
                    false,
                    TaskTypeDTO::Project});
    plannerTree.insert(std::ranges::find(plannerTree, "4", &PlannerItem::uuid),
                       PlannerItem{"11",
                                   {"Sub task 1", textColor, defaultBackground},
                                   {"Tag3", tagColor, defaultBackground},
                                   {"0/3", textColor, defaultBackground},
                                   anchorTime,
                                   {"", textColor, defaultBackground},
                                   "",
                                   {"", textColor, defaultBackground},
                                   false,
                                   false,
                                   TaskTypeDTO::Regular});
    plannerTree.insert(
        std::ranges::find(plannerTree, "11", &PlannerItem::uuid),
        PlannerItem{"12",
                    {"Sub project 1", tagColor, defaultBackground},
                    {"Tag2", tagColor, defaultBackground},
                    {"", textColor, defaultBackground},
                    anchorTime,
                    {"", textColor, defaultBackground},
                    "",
                    {"", textColor, defaultBackground},
                    false,
                    false,
                    TaskTypeDTO::Project});
    plannerTree.insert(std::ranges::find(plannerTree, "12", &PlannerItem::uuid),
                       PlannerItem{"13",
                                   {"Sub task 2", textColor, defaultBackground},
                                   {"Tag7", tagColor, defaultBackground},
                                   {"2/3", textColor, defaultBackground},
                                   anchorTime,
                                   {"", textColor, defaultBackground},
                                   "",
                                   {"", textColor, defaultBackground},
                                   true,
                                   false,
                                   TaskTypeDTO::Regular});
    plannerTree.insert(std::ranges::find(plannerTree, "12", &PlannerItem::uuid),
                       PlannerItem{"14",
                                   {"Sub task 3", textColor, defaultBackground},
                                   {"Tag8", tagColor, defaultBackground},
                                   {"2/2", doneWorkColor, defaultBackground},
                                   anchorTime,
                                   {"", textColor, defaultBackground},
                                   "",
                                   {"", textColor, defaultBackground},
                                   false,
                                   false,
                                   TaskTypeDTO::Regular});
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_overdue_task)
{
    TaskTreeDTO taskTree;
    taskTree.insert(taskTree.end(),
                    TaskDTO{"5",
                            {"Tag5", "Tag1"},
                            "Overdue task",
                            8,
                            {},
                            false,
                            anchorTime,
                            std::nullopt,
                            TaskTimeframeDTO{anchorTime - Days{5},
                                             anchorTime - Days{3},
                                             std::nullopt,
                                             std::nullopt},
                            TaskTypeDTO::Regular});
    PlannerTree plannerTree;
    plannerTree.insert(
        plannerTree.end(),
        PlannerItem{"5",
                    {"Overdue task", textColor, defaultBackground},
                    {"Tag5, Tag1", tagColor, defaultBackground},
                    {"0/8", textColor, defaultBackground},
                    anchorTime - Days{5},
                    {"-3 Days", contrastColor, dueOverdueColor},
                    "",
                    {"", textColor, defaultBackground},
                    false,
                    false,
                    TaskTypeDTO::Regular});
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_done_task)
{
    TaskTreeDTO taskTree;
    taskTree.insert(
        taskTree.end(),
        TaskDTO{"6",
                {"Tag1", "Tag4"},
                "Work done task",
                2,
                {DateTimeRange{anchorTime, anchorTime},
                 DateTimeRange{anchorTime, anchorTime}},
                false,
                anchorTime,
                std::nullopt,
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    PlannerTree plannerTree;
    plannerTree.insert(
        plannerTree.end(),
        PlannerItem{"6",
                    {"Work done task", textColor, defaultBackground},
                    {"Tag1, Tag4", tagColor, defaultBackground},
                    {"2/2", doneWorkColor, defaultBackground},
                    anchorTime,
                    {"", textColor, defaultBackground},
                    "",
                    {"", textColor, defaultBackground},
                    false,
                    false,
                    TaskTypeDTO::Regular});
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_overwork_task)
{
    TaskTreeDTO taskTree;
    taskTree.insert(
        taskTree.end(),
        TaskDTO{"7",
                {"Tag3", "Tag4"},
                "Overwork task",
                1,
                {DateTimeRange{anchorTime, anchorTime},
                 DateTimeRange{anchorTime, anchorTime}},
                false,
                anchorTime,
                std::nullopt,
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    PlannerTree plannerTree;
    plannerTree.insert(
        plannerTree.end(),
        PlannerItem{"7",
                    {"Overwork task", textColor, defaultBackground},
                    {"Tag3, Tag4", tagColor, defaultBackground},
                    {"2/1", overworkWorkColor, defaultBackground},
                    anchorTime,
                    {"", textColor, defaultBackground},
                    "",
                    {"", textColor, defaultBackground},
                    false,
                    false,
                    TaskTypeDTO::Regular});
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, saves_tree_when_nodes_are_moved)
{
    TaskTreeDTO taskTree;
    taskTree.insert(
        taskTree.end(),
        TaskDTO{"7",
                {"Tag3", "Tag4"},
                "Overwork task",
                1,
                {DateTimeRange{anchorTime, anchorTime},
                 DateTimeRange{anchorTime, anchorTime}},
                false,
                anchorTime,
                std::nullopt,
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    taskTree.insert(taskTree.end(),
                    TaskDTO{"1",
                            {"Tag1"},
                            "Other task",
                            4,
                            {DateTimeRange{anchorTime, anchorTime}},
                            false,
                            anchorTime,
                            std::nullopt,
                            TaskTimeframeDTO{anchorTime},
                            TaskTypeDTO::Regular});
    TaskTreeDTO expected;
    expected.insert(expected.end(),
                    TaskDTO{"1",
                            {"Tag1"},
                            "Other task",
                            4,
                            {DateTimeRange{anchorTime, anchorTime}},
                            false,
                            anchorTime,
                            std::nullopt,
                            TaskTimeframeDTO{anchorTime},
                            TaskTypeDTO::Regular});
    expected.insert(
        expected.end(),
        TaskDTO{"7",
                {"Tag3", "Tag4"},
                "Overwork task",
                1,
                {DateTimeRange{anchorTime, anchorTime},
                 DateTimeRange{anchorTime, anchorTime}},
                false,
                anchorTime,
                std::nullopt,
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));
    sut.attachView(view);

    EXPECT_CALL(savePlannerHandler, handle(SaveTaskTreeCommand{expected}));

    sut.moveNodes(std::nullopt, 0, 1, std::nullopt, 2);
}

TEST_F(PlannerPresenterFixture, delegates_deleting_task)
{
    EXPECT_CALL(deleteTaskHandler, handle(DeleteTaskCommand{"1234"}));

    sut.deleteTask("1234");
}

TEST_F(PlannerPresenterFixture, changes_task_addition_context)
{
    sprint_timer::ui::AddTaskContext expected{
        "123", sprint_timer::ui::TaskAddMode::Subtask};

    sut.changeTaskAdditionContext("123", true);

    EXPECT_EQ(expected, addTaskContext);
}

TEST_F(PlannerPresenterFixture, changes_task_edition_context)
{
    TaskDTO task{"123",
                 {"Tag1", "Tag2"},
                 "Some task that is due soon",
                 5,
                 {},
                 false,
                 dw::current_date_time(),
                 NoteDTO{"Just some text note"},
                 TaskTimeframeDTO{anchorTime},
                 TaskTypeDTO::Regular};
    sprint_timer::ui::EditTaskContext expected{TaskDTO{task}};
    TaskTreeDTO taskTree;
    taskTree.insert(std::ranges::find(taskTree, "123", &TaskDTO::uuid), task);
    ON_CALL(readPlannerHandler, handle(_)).WillByDefault(Return(taskTree));
    sut.attachView(view);

    sut.changeTaskEditionContext("123");

    EXPECT_EQ(expected, editTaskContext);
}

TEST_F(PlannerPresenterFixture,
       throws_when_failing_to_find_task_for_quick_edition)
{
    TaskTreeDTO taskTree;
    taskTree.insert(
        taskTree.end(),
        TaskDTO{"6",
                {"Tag1", "Tag4"},
                "Some task name",
                2,
                {DateTimeRange{anchorTime, anchorTime},
                 DateTimeRange{anchorTime, anchorTime}},
                false,
                anchorTime,
                std::nullopt,
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    TaskDTO expected{
        "6",
        {"Tag1", "Tag4"},
        "Changed task name",
        2,
        {DateTimeRange{anchorTime, anchorTime},
         DateTimeRange{anchorTime, anchorTime}},
        false,
        anchorTime,
        std::nullopt,
        TaskTimeframeDTO{anchorTime, std::nullopt, std::nullopt, std::nullopt},
        TaskTypeDTO::Regular};
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));
    sut.attachView(view);

    EXPECT_THROW(
        sut.quickEditTask(
            "777", std::string{"Some another name"}, {"Some_another_tag"}, 22),
        std::runtime_error);
}

TEST_F(PlannerPresenterFixture, relays_task_name_edition)
{
    TaskTreeDTO taskTree;
    taskTree.insert(
        taskTree.end(),
        TaskDTO{"6",
                {"Tag1", "Tag4"},
                "Some task name",
                2,
                {DateTimeRange{anchorTime, anchorTime},
                 DateTimeRange{anchorTime, anchorTime}},
                false,
                anchorTime,
                std::nullopt,
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    TaskDTO expected{
        "6",
        {"ChangedTag1"},
        "Changed task name",
        222,
        {DateTimeRange{anchorTime, anchorTime},
         DateTimeRange{anchorTime, anchorTime}},
        false,
        anchorTime,
        std::nullopt,
        TaskTimeframeDTO{anchorTime, std::nullopt, std::nullopt, std::nullopt},
        TaskTypeDTO::Regular};
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));
    sut.attachView(view);

    EXPECT_CALL(editTaskHandler, handle(EditTaskCommand{expected}));

    sut.quickEditTask(
        "6", std::string{"Changed task name"}, {"ChangedTag1"}, 222);
}

TEST_F(PlannerPresenterFixture, relays_task_toggle)
{
    TaskTreeDTO taskTree;
    taskTree.insert(
        taskTree.end(),
        TaskDTO{"6",
                {"Tag1", "Tag4"},
                "Some task name",
                2,
                {DateTimeRange{anchorTime, anchorTime},
                 DateTimeRange{anchorTime, anchorTime}},
                false,
                anchorTime,
                std::nullopt,
                TaskTimeframeDTO{
                    anchorTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));
    sut.attachView(view);

    EXPECT_CALL(toggleTaskHandler, handle(ToggleTaskCompletedCommand{"6"}));

    sut.toggleTask("6");
}

TEST_F(PlannerPresenterFixture, updates_view_when_new_filter_is_installed)
{
    setupSampleTree();
    sut.attachView(view);

    EXPECT_CALL(
        view,
        displayPlanner(ElementsAre(
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project2.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.folder3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project2.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.folder3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project4.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task2.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task4.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task5.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.recurringTask1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task6.uuid))));

    taskTreeFilter.select("Projects");
}

TEST_F(PlannerPresenterFixture,
       applies_installed_filter_when_updating_view_by_external_call)
{
    setupSampleTree();
    sut.attachView(view);
    taskTreeFilter.select("Projects");

    EXPECT_CALL(
        view,
        displayPlanner(ElementsAre(
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project2.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.folder3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project2.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.folder3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project4.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task2.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task4.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task5.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.recurringTask1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task6.uuid))));

    sut.fetchData();
    sut.updateView();
}

TEST_F(PlannerPresenterFixture,
       updates_view_with_unfiltered_tree_when_filter_is_cleared)
{
    setupSampleTree();
    sut.attachView(view);
    taskTreeFilter.select("Projects");

    EXPECT_CALL(
        view,
        displayPlanner(ElementsAre(
            Field(&PlannerItem::uuid, taskDtoTreeFixture.folder1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.folder2.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project2.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.folder3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project4.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task2.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task4.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task5.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.recurringTask1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task6.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task7.uuid))));

    taskTreeFilter.clear();
}

TEST_F(PlannerPresenterFixture, applies_active_actions_predefined_filter)
{
    setupSampleTree();
    sut.attachView(view);

    EXPECT_CALL(
        view,
        displayPlanner(UnorderedElementsAre(
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project2.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.recurringTask1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task7.uuid))));

    taskTreeFilter.select("Active Actions");
}

TEST_F(PlannerPresenterFixture, applies_active_actions_predefined_filter_2)
{
    auto taskTree = buildSampleTree();
    std::ranges::find(taskTree, taskDtoTreeFixture.task1.uuid, &TaskDTO::uuid)
        ->finished = true;
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));
    sut.attachView(view);
    EXPECT_CALL(
        view,
        displayPlanner(UnorderedElementsAre(
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project2.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.project3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task3.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.recurringTask1.uuid),
            Field(&PlannerItem::uuid, taskDtoTreeFixture.task7.uuid))));

    taskTreeFilter.select("Active Actions");
}

TEST_F(PlannerPresenterFixture, applies_active_actions_by_project_filter)
{
    /*
     * Project: project1
     *    project2
     * Project: project3
     *    task1
     * Project: project4
     *    task3
     *    recurringTask1
     * Project: None
     *    task7
     */
    setupSampleTree();
    sut.attachView(view);
    EXPECT_CALL(
        view,
        displayPlanner(ResultOf(
            get_subchildren,
            UnorderedElementsAre(
                Pair(
                    Field(&PlannerItem::name,
                          Item{"Project: " + taskDtoTreeFixture.project1.name +
                                   "  [1]",
                               tagColor,
                               defaultBackground}),
                    UnorderedElementsAre(Field(
                        &PlannerItem::uuid, taskDtoTreeFixture.project2.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Project: " + taskDtoTreeFixture.project3.name +
                                    "  [1]",
                                tagColor,
                                defaultBackground}),
                     UnorderedElementsAre(Field(
                         &PlannerItem::uuid, taskDtoTreeFixture.task1.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Project: " + taskDtoTreeFixture.project4.name +
                                    "  [2]",
                                tagColor,
                                defaultBackground}),
                     UnorderedElementsAre(
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.task3.uuid),
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.recurringTask1.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Project: None  [1]",
                                tagColor,
                                defaultBackground}),
                     UnorderedElementsAre(
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.task7.uuid)))))));

    try {
        taskTreeFilter.select("Active By Project");
    }
    catch (std::exception& exc) {
        throw;
    }
}

TEST_F(PlannerPresenterFixture,
       predefined_projects_filter_handles_active_project)
{
    auto taskTree = buildSampleTree();
    // Mark all subtree of folder3 as finished
    taskTree.map(std::ranges::find(
                     taskTree, taskDtoTreeFixture.folder3.uuid, &TaskDTO::uuid),
                 [](auto& payload) { payload.finished = true; });
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));
    sut.attachView(view);
    EXPECT_CALL(
        view,
        displayPlanner(ResultOf(
            get_subchildren,
            UnorderedElementsAre(
                Pair(Field(&PlannerItem::name,
                           Item{"Project: " + taskDtoTreeFixture.project1.name +
                                    "  [2]",
                                tagColor,
                                defaultBackground}),
                     UnorderedElementsAre(
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.project2.uuid),
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.project3.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Project: " + taskDtoTreeFixture.project4.name +
                                    "  [2]",
                                tagColor,
                                defaultBackground}),
                     UnorderedElementsAre(
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.task3.uuid),
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.recurringTask1.uuid)

                             )),
                Pair(Field(&PlannerItem::name,
                           Item{"Project: None  [1]",
                                tagColor,
                                defaultBackground}),
                     UnorderedElementsAre(
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.task7.uuid)))))));

    taskTreeFilter.select("Active By Project");
}

TEST_F(PlannerPresenterFixture, applies_active_actions_by_tag_predefined_filter)
{
    /*
     * folder1
     *    folder2
     *       project1
     *          project2
     *          project3
     *             folder3
     *                task1 Tag1
     * project4 Tag2
     *    task2 Tag2
     *       task3 Tag3
     *       task4 Tag4
     *          task5 Tag2, Tag5
     *          recurringTask1 Tag1
     *    task6 Tag6
     * task7 Tag7
     */

    /* Possible arrangement -- header order and order of tasks in headers are
     * irrelevant
     *
     *   No tag
     *     project2
     *   Tag1
     *     task1
     *     recurringTask1
     *   Tag3
     *     task3
     *     recurringTask1
     *   Tag7
     *     task7
     */
    setupSampleTree();
    sut.attachView(view);

    EXPECT_CALL(
        view,
        displayPlanner(ResultOf(
            get_subchildren,
            UnorderedElementsAre(
                Pair(
                    Field(&PlannerItem::name,
                          Item{"Tag: None  [1]", tagColor, defaultBackground}),
                    UnorderedElementsAre(Field(
                        &PlannerItem::uuid, taskDtoTreeFixture.project2.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Tag: Tag1  [2]", tagColor, defaultBackground}),
                     UnorderedElementsAre(
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.task1.uuid),
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.recurringTask1.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Tag: Tag3  [2]", tagColor, defaultBackground}),
                     UnorderedElementsAre(
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.task3.uuid),
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.recurringTask1.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Tag: Tag7  [1]", tagColor, defaultBackground}),
                     UnorderedElementsAre(
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.task7.uuid)))))));

    taskTreeFilter.select("Active By Tag");
}

TEST_F(PlannerPresenterFixture,
       active_actions_by_tag_filter_handles_case_when_all_tasks_are_tagged)
{
    auto initial_tree = buildSampleTree();
    TaskTreeDTO tree = initial_tree.subtree(std::ranges::find(
        initial_tree, taskDtoTreeFixture.task4.uuid, &TaskDTO::uuid));
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(tree));
    sut.attachView(view);
    EXPECT_CALL(
        view,
        displayPlanner(ResultOf(
            get_subchildren,
            UnorderedElementsAre(
                Pair(Field(&PlannerItem::name,
                           Item{"Tag: Tag3  [1]", tagColor, defaultBackground}),
                     UnorderedElementsAre(
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.recurringTask1.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Tag: Tag1  [1]", tagColor, defaultBackground}),
                     UnorderedElementsAre(
                         Field(&PlannerItem::uuid,
                               taskDtoTreeFixture.recurringTask1.uuid)))))));

    taskTreeFilter.select("Active By Tag");
}

TEST_F(PlannerPresenterFixture, recently_modified_predefined_filter)
{
    setupSampleTree();
    sut.attachView(view);

    /*

    Today

    project4
    task4
    folder3
    folder2
    folder1

    Yesterday

    task5
    project2

    Last week

    task3 -2
    recurringTask1 -4
    task2 -5
    task7 -6
    project1 -7

    Last month
    task1 -15
    project3 -30

    */

    EXPECT_CALL(
        view,
        displayPlanner(ResultOf(
            get_subchildren,
            ElementsAre(
                Pair(Field(&PlannerItem::name,
                           Item{"Modified: today (Sep 21)  [5]",
                                tagColor,
                                defaultBackground}),
                     ElementsAre(Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.project4.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task4.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.folder3.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.folder2.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.folder1.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Modified: yesterday (Sep 20)  [2]",
                                tagColor,
                                defaultBackground}),
                     ElementsAre(Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task5.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.project2.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Modified: few days ago (Sep 14 - Sep 19)  [5]",
                                tagColor,
                                defaultBackground}),
                     ElementsAre(Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task3.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.recurringTask1.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task2.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task7.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.project1.uuid))),
                Pair(
                    Field(&PlannerItem::name,
                          Item{"Modified: few weeks ago (Aug 22 - Sep 13)  [2]",
                               tagColor,
                               defaultBackground}),
                    ElementsAre(Field(&PlannerItem::uuid,
                                      taskDtoTreeFixture.task1.uuid),
                                Field(&PlannerItem::uuid,
                                      taskDtoTreeFixture.project3.uuid)))))));

    taskTreeFilter.select("Modified recently");
}

TEST_F(PlannerPresenterFixture, recently_finished_predefined_filter)
{
    auto tree = buildSampleTree();
    std::ranges::for_each(tree, [](auto& task) { task.finished = true; });
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(tree));
    sut.attachView(view);

    /*

    Today

    project4
    task4
    folder3
    folder2
    folder1

    Yesterday

    task5
    project2

    Last week

    task3 -2
    recurringTask1 -4
    task2 -5
    task7 -6
    project1 -7

    Last month
    task1 -15
    project3 -30

    */

    EXPECT_CALL(
        view,
        displayPlanner(ResultOf(
            get_subchildren,
            ElementsAre(
                Pair(Field(&PlannerItem::name,
                           Item{"Finished: today (Sep 21)  [5]",
                                tagColor,
                                defaultBackground}),
                     ElementsAre(Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.project4.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task4.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.folder3.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.folder2.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.folder1.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Finished: yesterday (Sep 20)  [2]",
                                tagColor,
                                defaultBackground}),
                     ElementsAre(Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task5.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.project2.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Finished: few days ago (Sep 14 - Sep 19)  [5]",
                                tagColor,
                                defaultBackground}),
                     ElementsAre(Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task3.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.recurringTask1.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task2.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task7.uuid),
                                 Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.project1.uuid))),
                Pair(
                    Field(&PlannerItem::name,
                          Item{"Finished: few weeks ago (Aug 22 - Sep 13)  [2]",
                               tagColor,
                               defaultBackground}),
                    ElementsAre(Field(&PlannerItem::uuid,
                                      taskDtoTreeFixture.task1.uuid),
                                Field(&PlannerItem::uuid,
                                      taskDtoTreeFixture.project3.uuid)))))));

    taskTreeFilter.select("Completed recently");
}

TEST_F(PlannerPresenterFixture, due_next_seven_days_predefined_filter)
{
    auto tree = buildSampleTree();
    auto set_due_date = [&](const auto& uuid, dw::DateTime due) {
        auto it = std::ranges::find(tree, uuid, &TaskDTO::uuid);
        it->timeFrame = TaskTimeframeDTO{
            anchorTime - Days{10}, due, std::nullopt, std::nullopt};
    };
    // These are all active actions
    set_due_date(taskDtoTreeFixture.project2.uuid,
                 anchorTime - Days{10}); // Should still be visible as overdue
    set_due_date(taskDtoTreeFixture.task1.uuid, anchorTime + Days{7});
    set_due_date(taskDtoTreeFixture.task3.uuid, anchorTime);
    set_due_date(taskDtoTreeFixture.recurringTask1.uuid,
                 anchorTime + Days{8}); // Should NOT be visible
    set_due_date(taskDtoTreeFixture.task7.uuid,
                 anchorTime - Days{8}); // Should be visible
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(tree));
    sut.attachView(view);

    EXPECT_CALL(
        view,
        displayPlanner(ResultOf(
            get_subchildren,
            ElementsAre(
                Pair(Field(&PlannerItem::name,
                           Item{"Sep 11 2022 (Sun)  [1]",
                                tagColor,
                                defaultBackground}),
                     ElementsAre(Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.project2.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Sep 13 2022 (Tue)  [1]",
                                tagColor,
                                defaultBackground}),
                     ElementsAre(Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task7.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Sep 21 2022 (Wed)  [1]",
                                tagColor,
                                defaultBackground}),
                     ElementsAre(Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task3.uuid))),
                Pair(Field(&PlannerItem::name,
                           Item{"Sep 28 2022 (Wed)  [1]",
                                tagColor,
                                defaultBackground}),
                     ElementsAre(Field(&PlannerItem::uuid,
                                       taskDtoTreeFixture.task1.uuid)))))));

    taskTreeFilter.select("Due next 7 days");
}
