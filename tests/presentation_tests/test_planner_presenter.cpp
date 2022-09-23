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
#include "api/requests/CreateTaskCommand.h"
#include "api/requests/DeleteTaskCommand.h"
#include "api/requests/EditTaskCommand.h"
#include "api/requests/ReadTaskTreeQuery.h"
#include "api/requests/SaveTaskTreeCommand.h"
#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"
#include "qt_gui/presentation/PlannerPresenter.h"
#include "gmock/gmock.h"

namespace {

using sprint_timer::Tree;
using sprint_timer::api::TaskTreeDTO;
using sprint_timer::ui::contracts::PlannerContract::PlannerItem;

constexpr std::string_view defaultBackround{"defaultBackground"};
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

} // namespace

using sprint_timer::Tree;
using sprint_timer::ui::PlannerColors;
using sprint_timer::ui::contracts::PlannerContract::PlannerItem;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using namespace sprint_timer::api;
using namespace dw;
using namespace std::chrono_literals;

class PlannerWindowMock
    : public sprint_timer::ui::contracts::PlannerContract::View {
public:
    MOCK_METHOD(void,
                displayPlanner,
                ((const Tree<std::string, PlannerItem>&)),
                (override));
};

class MockDateTimeProvider : public DateTimeProvider {
public:
    MOCK_METHOD(dw::Date, dateNow, (), (const override));

    MOCK_METHOD(dw::DateTime, dateTimeNow, (), (const override));

    MOCK_METHOD(dw::Date, dateLocalNow, (), (const override));

    MOCK_METHOD(dw::DateTime, dateTimeLocalNow, (), (const override));
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
    NiceMock<MockDateTimeProvider> dateTimeProvider;
    sprint_timer::ui::AddTaskContext addTaskContext;
    sprint_timer::ui::EditTaskContext editTaskContext;
    PlannerColors plannerColors{textColor,
                                contrastColor,
                                defaultBackround,
                                normalWorkColor,
                                doneWorkColor,
                                overworkWorkColor,
                                dueNotSoonColor,
                                dueSoonColor,
                                dueTodayColor,
                                dueOverdueColor,
                                tagColor};
    sprint_timer::ui::PlannerPresenter sut{plannerColors,
                                           readPlannerHandler,
                                           savePlannerHandler,
                                           deleteTaskHandler,
                                           addTaskContext,
                                           editTaskContext,
                                           dateTimeProvider};
};

TEST_F(PlannerPresenterFixture, does_nothing_when_view_is_not_attached)
{
    EXPECT_CALL(view, displayPlanner(_)).Times(0);

    sut.updateView();
}

TEST_F(PlannerPresenterFixture, updates_view_when_view_is_attached)
{
    auto matches_tree = [](const Tree<std::string, PlannerItem>& tree) {
        return tree == Tree<std::string, PlannerItem>{};
    };
    ON_CALL(dateTimeProvider, dateTimeLocalNow)
        .WillByDefault(Return(dw::current_date_time_local()));
    ON_CALL(readPlannerHandler, handle(sprint_timer::api::ReadTaskTreeQuery{}))
        .WillByDefault(
            Return(::testing::ByMove(sprint_timer::api::TaskTreeDTO{})));

    EXPECT_CALL(view, displayPlanner(::testing::Truly(matches_tree)));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, updates_view)
{
    using namespace sprint_timer::api;
    const DateTime anchorTime{DateTime{Date{Year{2022}, Month{9}, Day{21}}} +
                              10h};
    TaskTreeDTO taskTree;
    taskTree.addChild(
        "1",
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
                TaskTypeDTO::Regular},
        std::nullopt);
    Tree<std::string, PlannerItem> plannerTree;
    plannerTree.addChild(
        "1",
        PlannerItem{"1",
                    {"Some root task", textColor, defaultBackround},
                    {"Tag9, Tag2", tagColor, defaultBackround},
                    {"0/2", textColor, defaultBackround},
                    anchorTime,
                    {"", textColor, defaultBackround},
                    "Just some text note",
                    {"", textColor, defaultBackround},
                    false,
                    TaskTypeDTO::Regular},
        std::nullopt);
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));
    ON_CALL(dateTimeProvider, dateTimeLocalNow)
        .WillByDefault(Return(dw::current_date_time_local()));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_task_without_time_frame)
{
    const DateTime anchorTime{DateTime{Date{Year{2022}, Month{9}, Day{21}}} +
                              10h};
    TaskTreeDTO taskTree;
    taskTree.addChild(
        "1",
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
                TaskTypeDTO::Regular},
        std::nullopt);
    Tree<std::string, PlannerItem> plannerTree;
    plannerTree.addChild(
        "1",
        PlannerItem{"1",
                    {"Some root task", textColor, defaultBackround},
                    {"Tag9, Tag2", tagColor, defaultBackround},
                    {"0/2", textColor, defaultBackround},
                    anchorTime,
                    {"", textColor, defaultBackround},
                    "Just some text note",
                    {"", textColor, defaultBackround},
                    false,
                    TaskTypeDTO::Regular},
        std::nullopt);
    ON_CALL(dateTimeProvider, dateTimeLocalNow)
        .WillByDefault(Return(anchorTime));
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_task_that_is_due_soon)
{
    const DateTime anchorTime{DateTime{Date{Year{2022}, Month{9}, Day{21}}} +
                              10h};
    TaskTreeDTO taskTree;
    taskTree.addChild("2",
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
                              TaskTypeDTO::Regular},
                      std::nullopt);
    Tree<std::string, PlannerItem> plannerTree;
    plannerTree.addChild(
        "2",
        PlannerItem{"2",
                    {"Some task that is due soon", textColor, defaultBackround},
                    {"Tag1, Tag2", tagColor, defaultBackround},
                    {"0/5", textColor, defaultBackround},
                    anchorTime - Days{2},
                    {"Saturday", contrastColor, dueSoonColor},
                    "Just some text note",
                    {"", textColor, defaultBackround},
                    false,
                    TaskTypeDTO::Regular},
        std::nullopt);
    ON_CALL(dateTimeProvider, dateTimeLocalNow)
        .WillByDefault(Return(anchorTime));
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_task_with_reminder)
{
    const DateTime anchorTime{DateTime{Date{Year{2022}, Month{9}, Day{21}}} +
                              10h};
    TaskTreeDTO taskTree;
    taskTree.addChild("3",
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
                              TaskTypeDTO::Regular},
                      std::nullopt);
    Tree<std::string, PlannerItem> plannerTree;
    plannerTree.addChild(
        "3",
        PlannerItem{"3",
                    {"Some task with reminder", textColor, defaultBackround},
                    {"Tag1, Tag2", tagColor, defaultBackround},
                    {"0/7", textColor, defaultBackround},
                    anchorTime - Days{2},
                    {"+5 Days", contrastColor, dueNotSoonColor},
                    "Task with reminder note",
                    {"08:00 26.09.2022", textColor, defaultBackround},
                    false,
                    TaskTypeDTO::Regular},
        std::nullopt);
    ON_CALL(dateTimeProvider, dateTimeLocalNow)
        .WillByDefault(Return(anchorTime));
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_project)
{
    const DateTime anchorTime{DateTime{Date{Year{2022}, Month{9}, Day{21}}} +
                              10h};
    TaskTreeDTO taskTree;
    taskTree.addChild("4",
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
                              TaskTypeDTO::Project},
                      std::nullopt);
    taskTree.addChild(
        "11",
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
                TaskTypeDTO::Regular},
        "4");
    taskTree.addChild(
        "12",
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
                TaskTypeDTO::Project},
        "11");
    taskTree.addChild(
        "13",
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
                TaskTypeDTO::Regular},
        "12");
    taskTree.addChild(
        "14",
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
                TaskTypeDTO::Regular},
        "12");
    Tree<std::string, PlannerItem> plannerTree;
    plannerTree.addChild(
        "4",
        PlannerItem{"4",
                    {"Project", tagColor, defaultBackround},
                    {"Tag1, Tag2", tagColor, defaultBackround},
                    {"", textColor, defaultBackround},
                    anchorTime - Days{2},
                    {"+5 Days", contrastColor, dueNotSoonColor},
                    "Task with reminder note",
                    {"08:00 26.09.2022", textColor, defaultBackround},
                    false,
                    TaskTypeDTO::Project},
        std::nullopt);
    plannerTree.addChild(
        "11",
        PlannerItem{"11",
                    {"Sub task 1", textColor, defaultBackround},
                    {"Tag3", tagColor, defaultBackround},
                    {"0/3", textColor, defaultBackround},
                    anchorTime,
                    {"", textColor, defaultBackround},
                    "",
                    {"", textColor, defaultBackround},
                    false,
                    TaskTypeDTO::Regular},
        "4");
    plannerTree.addChild(
        "12",
        PlannerItem{"12",
                    {"Sub project 1", tagColor, defaultBackround},
                    {"Tag2", tagColor, defaultBackround},
                    {"", textColor, defaultBackround},
                    anchorTime,
                    {"", textColor, defaultBackround},
                    "",
                    {"", textColor, defaultBackround},
                    false,
                    TaskTypeDTO::Project},
        "11");
    plannerTree.addChild(
        "13",
        PlannerItem{"13",
                    {"Sub task 2", textColor, defaultBackround},
                    {"Tag7", tagColor, defaultBackround},
                    {"2/3", textColor, defaultBackround},
                    anchorTime,
                    {"", textColor, defaultBackround},
                    "",
                    {"", textColor, defaultBackround},
                    true,
                    TaskTypeDTO::Regular},
        "12");
    plannerTree.addChild(
        "14",
        PlannerItem{"14",
                    {"Sub task 3", textColor, defaultBackround},
                    {"Tag8", tagColor, defaultBackround},
                    {"2/2", doneWorkColor, defaultBackround},
                    anchorTime,
                    {"", textColor, defaultBackround},
                    "",
                    {"", textColor, defaultBackround},
                    false,
                    TaskTypeDTO::Regular},
        "12");
    ON_CALL(dateTimeProvider, dateTimeLocalNow)
        .WillByDefault(Return(anchorTime));
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_overdue_task)
{
    const DateTime anchorTime{DateTime{Date{Year{2022}, Month{9}, Day{21}}} +
                              10h};
    TaskTreeDTO taskTree;
    taskTree.addChild("5",
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
                              TaskTypeDTO::Regular},
                      std::nullopt);
    Tree<std::string, PlannerItem> plannerTree;
    plannerTree.addChild(
        "5",
        PlannerItem{"5",
                    {"Overdue task", textColor, defaultBackround},
                    {"Tag5, Tag1", tagColor, defaultBackround},
                    {"0/8", textColor, defaultBackround},
                    anchorTime - Days{5},
                    {"-3 Days", contrastColor, dueOverdueColor},
                    "",
                    {"", textColor, defaultBackround},
                    false,
                    TaskTypeDTO::Regular},
        std::nullopt);
    ON_CALL(dateTimeProvider, dateTimeLocalNow)
        .WillByDefault(Return(anchorTime));
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_done_task)
{
    const DateTime anchorTime{DateTime{Date{Year{2022}, Month{9}, Day{21}}} +
                              10h};
    TaskTreeDTO taskTree;
    taskTree.addChild(
        "6",
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
                TaskTypeDTO::Regular},
        std::nullopt);
    Tree<std::string, PlannerItem> plannerTree;
    plannerTree.addChild(
        "6",
        PlannerItem{"6",
                    {"Work done task", textColor, defaultBackround},
                    {"Tag1, Tag4", tagColor, defaultBackround},
                    {"2/2", doneWorkColor, defaultBackround},
                    anchorTime,
                    {"", textColor, defaultBackround},
                    "",
                    {"", textColor, defaultBackround},
                    false,
                    TaskTypeDTO::Regular},
        std::nullopt);
    ON_CALL(dateTimeProvider, dateTimeLocalNow)
        .WillByDefault(Return(anchorTime));
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, displays_overwork_task)
{
    const DateTime anchorTime{DateTime{Date{Year{2022}, Month{9}, Day{21}}} +
                              10h};
    TaskTreeDTO taskTree;
    taskTree.addChild(
        "7",
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
                TaskTypeDTO::Regular},
        std::nullopt);
    Tree<std::string, PlannerItem> plannerTree;
    plannerTree.addChild(
        "7",
        PlannerItem{"7",
                    {"Overwork task", textColor, defaultBackround},
                    {"Tag3, Tag4", tagColor, defaultBackround},
                    {"2/1", overworkWorkColor, defaultBackround},
                    anchorTime,
                    {"", textColor, defaultBackround},
                    "",
                    {"", textColor, defaultBackround},
                    false,
                    TaskTypeDTO::Regular},
        std::nullopt);
    ON_CALL(dateTimeProvider, dateTimeLocalNow)
        .WillByDefault(Return(anchorTime));
    ON_CALL(readPlannerHandler, handle(ReadTaskTreeQuery{}))
        .WillByDefault(Return(taskTree));

    EXPECT_CALL(view, displayPlanner(plannerTree));

    sut.attachView(view);
}

TEST_F(PlannerPresenterFixture, saves_tree_when_nodes_are_moved)
{
    const DateTime anchorTime{DateTime{Date{Year{2022}, Month{9}, Day{21}}} +
                              10h};
    TaskTreeDTO taskTree;
    taskTree.addChild(
        "7",
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
                TaskTypeDTO::Regular},
        std::nullopt);
    taskTree.addChild("1",
                      TaskDTO{"1",
                              {"Tag1"},
                              "Other task",
                              4,
                              {DateTimeRange{anchorTime, anchorTime}},
                              false,
                              anchorTime,
                              std::nullopt,
                              TaskTimeframeDTO{},
                              TaskTypeDTO::Regular},
                      std::nullopt);
    TaskTreeDTO expected;
    expected.addChild("1",
                      TaskDTO{"1",
                              {"Tag1"},
                              "Other task",
                              4,
                              {DateTimeRange{anchorTime, anchorTime}},
                              false,
                              anchorTime,
                              std::nullopt,
                              TaskTimeframeDTO{},
                              TaskTypeDTO::Regular},
                      std::nullopt);
    expected.addChild(
        "7",
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
                TaskTypeDTO::Regular},
        std::nullopt);
    ON_CALL(dateTimeProvider, dateTimeLocalNow)
        .WillByDefault(Return(anchorTime));
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
                 TaskTimeframeDTO{},
                 TaskTypeDTO::Regular};
    sprint_timer::ui::EditTaskContext expected{TaskDTO{task}};
    TaskTreeDTO taskTree;
    taskTree.addChild("123", task, std::nullopt);
    ON_CALL(dateTimeProvider, dateTimeLocalNow)
        .WillByDefault(Return(dw::current_date_time_local()));
    ON_CALL(readPlannerHandler, handle(_)).WillByDefault(Return(taskTree));
    sut.attachView(view);

    sut.changeTaskEditionContext("123");

    EXPECT_EQ(expected, editTaskContext);
}
