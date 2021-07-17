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
#include "core/use_cases/request_sprints/RequestSprintsQuery.h"
#include "core/use_cases/request_tasks/FinishedTasksQuery.h"
#include "mocks/HistoryMediatorMock.h"
#include "mocks/QueryHandlerMock.h"
#include "qt_gui/presentation/HistoryPresenter.h"

using namespace ::testing;
using namespace sprint_timer;
using namespace std::chrono_literals;
using namespace dw;

namespace sprint_timer::ui::contracts::HistoryContract {

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const Item& item)
{
    os << "Item{";
    os << item.description << ", ";
    os << item.uuid;
    os << '}';
    return os;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const DayHistory& history)
{
    os << "DayHistory{";
    os << history.date << ", ";
    for (const auto& element : history.sortedItems) {
        os << element << ", ";
    }
    os << "}\n";
    return os;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const History& history)
{
    os << "History{\n";
    os << "\ttotal: " << history.totalItems << "\n";
    os << "\t";
    for (const auto& element : history.sortedDayHistory) {
        os << element << ", ";
    }
    os << "}\n";
    os << "}\n";
    return os;
}

} // namespace sprint_timer::ui::contracts::HistoryContract

namespace {

std::vector<sprint_timer::use_cases::SprintDTO>
buildSomeSprints(const dw::Date& someDate);

std::vector<sprint_timer::use_cases::TaskDTO>
buildSomeTasks(const dw::Date& someDate);

} // namespace

class HistoryTabViewMock : public ui::contracts::HistoryContract::View {
public:
    MOCK_METHOD(void,
                displayHistory,
                (const ui::contracts::HistoryContract::History&),
                (override));

    MOCK_METHOD(ui::contracts::HistoryContract::TaskEditData,
                openEditTaskDialog,
                (const ui::contracts::HistoryContract::TaskEditData&),
                (override));

    MOCK_METHOD(ui::contracts::HistoryContract::SprintEditData,
                openEditSprintDialog,
                (const ui::contracts::HistoryContract::SprintEditData&),
                (override));
};

class HistoryPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::QueryHandlerMock<use_cases::RequestSprintsQuery>>
        sprintHandlerMock;
    NiceMock<mocks::QueryHandlerMock<use_cases::FinishedTasksQuery>>
        taskHandlerMock;
    NiceMock<mocks::HistoryMediatorMock> mediatorMock;
    ui::HistoryPresenter presenter{
        sprintHandlerMock, taskHandlerMock, mediatorMock};
    NiceMock<HistoryTabViewMock> viewMock;

    const dw::DateTime someTime{dw::Date{Year{2020}, Month{6}, Day{8}}};
    const dw::DateTimeRange someTimeRange{someTime + 2h, someTime + 2h + 25min};
};

TEST_F(HistoryPresenterFixture, does_nothing_when_view_is_not_attached)
{
    ON_CALL(mediatorMock, currentDateRange())
        .WillByDefault(Return(std::nullopt));
    ON_CALL(mediatorMock, displayedHistory())
        .WillByDefault(
            Return(ui::HistoryMediator::DisplayedHistory::SprintHistory));

    EXPECT_CALL(sprintHandlerMock, handle(_)).Times(0);
    EXPECT_CALL(taskHandlerMock, handle(_)).Times(0);
    EXPECT_CALL(viewMock, displayHistory(_)).Times(0);

    presenter.updateView();
}

TEST_F(HistoryPresenterFixture, does_nothing_when_no_date_range_is_provided)
{
    ON_CALL(mediatorMock, currentDateRange())
        .WillByDefault(Return(std::nullopt));
    ON_CALL(mediatorMock, displayedHistory())
        .WillByDefault(
            Return(ui::HistoryMediator::DisplayedHistory::SprintHistory));

    EXPECT_CALL(viewMock, displayHistory(_)).Times(0);

    presenter.attachView(viewMock);
}

TEST_F(HistoryPresenterFixture,
       displays_sprint_history_when_sprint_should_be_displayed)
{
    using namespace sprint_timer::ui::contracts::HistoryContract;
    const dw::DateRange dateRange{dw::current_date(), dw::current_date()};
    const Date someDate{Date{Year{2020}, Month{6}, Day{8}}};
    // NOTE that HistoryPresenter assumes that sprints are sorted by their
    // startTime in ascending order
    const auto sprints = buildSomeSprints(someDate);
    const auto expected = History{
        7,
        {DayHistory{someDate - Days{3},
                    {Item{"04:00 - 04:25 #Tag3 Another task", "1"}}},
         DayHistory{someDate - Days{2},
                    {Item{"03:00 - 03:25 #Tag1 #Tag2 Some task", "2"}}},
         DayHistory{someDate,
                    {Item{"01:00 - 01:25 #Tag1 #Tag2 Some task", "3"},
                     Item{"10:00 - 10:25 #Tag1 #Tag2 Some task", "4"}}},
         DayHistory{someDate + Days{2},
                    {
                        Item{"01:00 - 01:25 #Tag1 #Tag2 Some task", "5"},
                        Item{"06:00 - 06:25 #Tag1 #Tag2 Some task", "6"},
                    }},
         DayHistory{someDate + Days{3},
                    {Item{"05:00 - 05:25 #Tag3 Another task", "7"}}}}};
    ON_CALL(mediatorMock, currentDateRange()).WillByDefault(Return(dateRange));
    ON_CALL(mediatorMock, displayedHistory())
        .WillByDefault(
            Return(ui::HistoryMediator::DisplayedHistory::SprintHistory));
    ON_CALL(sprintHandlerMock, handle(_)).WillByDefault(Return(sprints));

    EXPECT_CALL(viewMock, displayHistory(expected));

    presenter.attachView(viewMock);
}

TEST_F(HistoryPresenterFixture,
       displays_task_history_when_task_history_should_be_displayed)
{
    using namespace sprint_timer::ui::contracts::HistoryContract;
    const dw::Date someDate{dw::Date{Year{2020}, Month{6}, Day{8}}};
    const auto someTasks = buildSomeTasks(someDate);
    const History expected{
        7,
        {DayHistory{someDate - Days{4}, {Item{"#Tag1 Earliest task 3/2", "1"}}},
         DayHistory{someDate - Days{3},
                    {Item{"#Tag9 Second to earliest 7/2", "2"}}},
         DayHistory{someDate - Days{2},
                    {Item{"#Tag7 Same day task 1 15/5", "3"},
                     Item{"#Tag5 Same day task 2 5/5", "4"},
                     Item{"#Tag5 Same day task 3 7/12", "5"}}},
         DayHistory{someDate + Days{3},
                    {Item{"#Tag1 Second to last 5/5", "6"}}},
         DayHistory{someDate + Days{4}, {Item{"Latest task 0/7", "7"}}}}};
    const dw::DateRange dateRange{dw::current_date(), dw::current_date()};
    ON_CALL(mediatorMock, currentDateRange()).WillByDefault(Return(dateRange));
    ON_CALL(mediatorMock, displayedHistory())
        .WillByDefault(
            Return(ui::HistoryMediator::DisplayedHistory::TaskHistory));
    ON_CALL(taskHandlerMock, handle(_)).WillByDefault(Return(someTasks));

    EXPECT_CALL(viewMock, displayHistory(expected));

    presenter.attachView(viewMock);
}

TEST_F(HistoryPresenterFixture,
       notifies_mediator_when_displayed_tab_is_changed_to_sprint_tab)
{
    EXPECT_CALL(
        mediatorMock,
        changeDisplayedHistory(
            &presenter,
            mocks::HistoryMediatorMock::DisplayedHistory::SprintHistory));

    presenter.onDisplayedTabChanged(0);
}

TEST_F(HistoryPresenterFixture,
       notifies_mediator_when_displayed_tab_is_changed_to_task_tab)
{
    EXPECT_CALL(mediatorMock,
                changeDisplayedHistory(
                    &presenter,
                    mocks::HistoryMediatorMock::DisplayedHistory::TaskHistory));

    presenter.onDisplayedTabChanged(1);
}

TEST_F(HistoryPresenterFixture,
       displays_history_when_notified_about_shared_data_change)
{
    const dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
    const std::vector<use_cases::SprintDTO> sprints;
    const ui::contracts::HistoryContract::History expected;
    presenter.attachView(viewMock);
    ON_CALL(mediatorMock, currentDateRange())
        .WillByDefault(Return(someDateRange));
    ON_CALL(mediatorMock, displayedHistory())
        .WillByDefault(
            Return(ui::HistoryMediator::DisplayedHistory::SprintHistory));
    ON_CALL(sprintHandlerMock, handle(_)).WillByDefault(Return(sprints));

    EXPECT_CALL(viewMock, displayHistory(expected));

    presenter.onSharedDataChanged();
}

namespace {

std::vector<sprint_timer::use_cases::SprintDTO>
buildSomeSprints(const dw::Date& someDate)
{
    using namespace dw;
    using sprint_timer::use_cases::SprintDTO;
    const DateTime someDateTime{someDate};
    const DateTimeRange span{someDateTime, someDateTime + 25min};
    std::vector<SprintDTO> sprints{
        SprintDTO{"1",
                  "345",
                  "Another task",
                  {"Tag3"},
                  add_offset(span, -Days{3} + 4h)},
        SprintDTO{"2",
                  "123",
                  "Some task",
                  {"Tag1", "Tag2"},
                  add_offset(span, -Days{2} + 3h)},
        SprintDTO{
            "3", "123", "Some task", {"Tag1", "Tag2"}, add_offset(span, 1h)},
        SprintDTO{
            "4", "123", "Some task", {"Tag1", "Tag2"}, add_offset(span, 10h)},
        SprintDTO{"5",
                  "123",
                  "Some task",
                  {"Tag1", "Tag2"},
                  add_offset(span, Days{2} + 1h)},
        SprintDTO{"6",
                  "123",
                  "Some task",
                  {"Tag1", "Tag2"},
                  add_offset(span, Days{2} + 6h)},
        SprintDTO{"7",
                  "345",
                  "Another task",
                  {"Tag3"},
                  add_offset(span, Days{3} + 5h)},
    };
    return sprints;
}

std::vector<sprint_timer::use_cases::TaskDTO>
buildSomeTasks(const dw::Date& someDate)
{
    using namespace sprint_timer;
    using namespace dw;
    using use_cases::TaskDTO;
    const DateTime someDateTime{someDate};
    std::vector<TaskDTO> tasks{
        TaskDTO{
            "1", {"Tag1"}, "Earliest task", 2, 3, true, someDateTime - Days{4}},
        TaskDTO{"2",
                {"Tag9"},
                "Second to earliest",
                2,
                7,
                true,
                someDateTime - Days{3} + 3h},
        TaskDTO{"3",
                {"Tag7"},
                "Same day task 1",
                5,
                15,
                true,
                someDateTime - Days{2}},
        TaskDTO{"4",
                {"Tag5"},
                "Same day task 2",
                5,
                5,
                true,
                someDateTime - Days{2} + 1h},
        TaskDTO{"5",
                {"Tag5"},
                "Same day task 3",
                12,
                7,
                true,
                someDateTime - Days{2} + 3h},
        TaskDTO{"6",
                {"Tag1"},
                "Second to last",
                5,
                5,
                true,
                someDateTime + Days{3}},
        TaskDTO{"7",
                std::vector<std::string>{},
                "Latest task",
                7,
                0,
                true,
                someDateTime + Days{4}}};
    return tasks;
}

} // namespace

