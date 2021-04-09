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
#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"
#include <gmock/gmock-spec-builders.h>
#include <qt_gui/presentation/WorkScheduleEditorPresenter.h>

using namespace sprint_timer;
using namespace ::testing;
using namespace std::chrono_literals;

class WorkScheduleEditorViewMock
    : public ui::contracts::WorkScheduleEditor::View {
public:
    MOCK_METHOD(
        void,
        displayCurrentWeekSchedule,
        (const std::vector<
            sprint_timer::ui::contracts::WorkScheduleEditor::DayAndGoal>&),
        (override));

    MOCK_METHOD(
        void,
        displayRoaster,
        (const std::vector<
            sprint_timer::ui::contracts::WorkScheduleEditor::RoasterRow>&),
        (override));

    MOCK_METHOD(void,
                displayExceptionalDays,
                (const std::vector<WorkSchedule::DateGoal>&),
                (override));

    MOCK_METHOD(void,
                displayAddExceptionalDaysDialog,
                (dw::Weekday, dw::Date),
                (override));
};

namespace sprint_timer::use_cases {

bool operator==(const ChangeWorkScheduleCommand& lhs,
                const ChangeWorkScheduleCommand& rhs)
{
    return lhs.oldSchedule == rhs.oldSchedule &&
           lhs.newSchedule == rhs.newSchedule;
}

} // namespace sprint_timer::use_cases

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, uint8_t v)
{
    os << static_cast<unsigned>(v);
    return os;
}

WeekSchedule buildWeekSchedule(const std::array<int, 7>& raw_schedule)
{
    WeekSchedule w_schedule;
    for (size_t i = 0; i < raw_schedule.size(); ++i) {
        w_schedule.setTargetGoal(static_cast<dw::Weekday>(i), raw_schedule[i]);
    }
    return w_schedule;
}

WorkSchedule createSomeWorkSchedule()
{
    using namespace dw;
    const WeekSchedule someWeekSchedule{
        buildWeekSchedule({10, 11, 12, 13, 14, 0, 0})};
    const Date holiday_1{Date{Year{2018}, Month{6}, Day{11}}};
    const Date holiday_2{Date{Year{2018}, Month{6}, Day{12}}};
    const Date extra_workday_1{Date{Year{2018}, Month{6}, Day{9}}};
    WorkSchedule schedule;
    schedule.addWeekSchedule(Date{Year{1996}, Month{1}, Day{1}},
                             someWeekSchedule);
    schedule.addExceptionalDay(holiday_1, 0);
    schedule.addExceptionalDay(holiday_2, 0);
    schedule.addExceptionalDay(extra_workday_1, 12);
    return schedule;
}

class WorkScheduleEditorPresenterFixture : public ::testing::Test {
public:
    NiceMock<WorkScheduleEditorViewMock> view;
    NiceMock<
        mocks::QueryHandlerMock<use_cases::WorkScheduleQuery, WorkSchedule>>
        workScheduleHandler;
    NiceMock<mocks::CommandHandlerMock<use_cases::ChangeWorkScheduleCommand>>
        changeWorkScheduleHandler;
    const dw::Weekday firstDayOfWeek{dw::Weekday::Monday};
    ui::WorkScheduleEditorPresenter presenter{
        workScheduleHandler, changeWorkScheduleHandler, firstDayOfWeek};
    WorkSchedule schedule{createSomeWorkSchedule()};
};

TEST_F(WorkScheduleEditorPresenterFixture,
       fills_current_week_schedule_when_view_is_attached)
{
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    const std::vector<ui::contracts::WorkScheduleEditor::DayAndGoal> expected{
        {1, 10}, {2, 11}, {3, 12}, {4, 13}, {5, 14}, {6, 0}, {7, 0}};

    EXPECT_CALL(view, displayCurrentWeekSchedule(expected));

    presenter.attachView(view);
}

TEST_F(WorkScheduleEditorPresenterFixture,
       constructs_current_week_schedule_depending_on_first_day_of_week)
{
    ui::WorkScheduleEditorPresenter sundayFirstPresenter{
        workScheduleHandler, changeWorkScheduleHandler, dw::Weekday::Sunday};
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    const std::vector<ui::contracts::WorkScheduleEditor::DayAndGoal> expected{
        {7, 0}, {1, 10}, {2, 11}, {3, 12}, {4, 13}, {5, 14}, {6, 0}};

    EXPECT_CALL(view, displayCurrentWeekSchedule(expected));

    sundayFirstPresenter.attachView(view);
}

TEST_F(WorkScheduleEditorPresenterFixture,
       fill_schedule_roaster_when_view_is_attached)
{
    using namespace dw;
    WeekSchedule otherWeekSchedule = buildWeekSchedule({1, 2, 3, 4, 5, 0, 0});
    schedule.addWeekSchedule(Date{Year{2000}, Month{3}, Day{4}},
                             otherWeekSchedule);
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    const std::vector<ui::contracts::WorkScheduleEditor::RoasterRow> expected{
        {Date{Year{1996}, Month{1}, Day{1}}, "10, 11, 12, 13, 14, 0, 0"},
        {Date{Year{2000}, Month{3}, Day{4}}, "1, 2, 3, 4, 5, 0, 0"}};

    EXPECT_CALL(view, displayRoaster(expected));

    presenter.attachView(view);
}

TEST_F(
    WorkScheduleEditorPresenterFixture,
    fills_schedule_roaster_when_view_is_attached_taking_in_account_first_day_of_week)
{
    using namespace dw;
    ui::WorkScheduleEditorPresenter sundayFirstPresenter{
        workScheduleHandler, changeWorkScheduleHandler, dw::Weekday::Sunday};
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    const std::vector<ui::contracts::WorkScheduleEditor::RoasterRow> expected{
        {Date{Year{1996}, Month{1}, Day{1}}, "0, 10, 11, 12, 13, 14, 0"}};

    EXPECT_CALL(view, displayRoaster(expected));

    sundayFirstPresenter.attachView(view);
}

TEST_F(WorkScheduleEditorPresenterFixture,
       fills_exceptional_days_sorted_asc_by_date_when_view_is_attached)
{
    using namespace dw;
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    const std::vector<WorkSchedule::DateGoal> exceptionalDays{
        {{Date{Year{2018}, Month{6}, Day{9}}}, 12},
        {{Date{Year{2018}, Month{6}, Day{11}}}, 0},
        {{Date{Year{2018}, Month{6}, Day{12}}}, 0}};

    EXPECT_CALL(view, displayExceptionalDays(exceptionalDays));

    presenter.attachView(view);
}

TEST_F(WorkScheduleEditorPresenterFixture,
       makes_view_display_dialog_when_requested_to_add_exceptional_days)
{
    presenter.attachView(view);

    EXPECT_CALL(view,
                displayAddExceptionalDaysDialog(dw::Weekday::Monday,
                                                dw::current_date_local()));

    presenter.onAddExceptionalRequested();
}

TEST_F(WorkScheduleEditorPresenterFixture,
       displays_updated_exceptional_days_when_exceptional_days_are_added)
{
    using namespace dw;
    const Date startDate{Year{2018}, Month{5}, Day{1}};
    const uint8_t numDays{4};
    const uint8_t sprintsPerDay{0};
    const std::vector<WorkSchedule::DateGoal> exceptionalDays{
        {{Date{Year{2018}, Month{5}, Day{1}}}, 0},
        {{Date{Year{2018}, Month{5}, Day{2}}}, 0},
        {{Date{Year{2018}, Month{5}, Day{3}}}, 0},
        {{Date{Year{2018}, Month{5}, Day{4}}}, 0},
        {{Date{Year{2018}, Month{6}, Day{9}}}, 12},
        {{Date{Year{2018}, Month{6}, Day{11}}}, 0},
        {{Date{Year{2018}, Month{6}, Day{12}}}, 0}};
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    presenter.attachView(view);

    EXPECT_CALL(view, displayExceptionalDays(exceptionalDays));

    presenter.onExceptionalDaysAdded(startDate, numDays, sprintsPerDay);
}

TEST_F(WorkScheduleEditorPresenterFixture,
       does_not_display_updated_exceptional_days_when_exceptional_days_removed)
{
    using namespace dw;
    const std::vector<WorkSchedule::DateGoal> exceptionalDays{
        {{Date{Year{2018}, Month{6}, Day{9}}}, 12},
        {{Date{Year{2018}, Month{6}, Day{12}}}, 0}};
    const Date removeCandidate{Year{2018}, Month{6}, Day{11}};
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    presenter.attachView(view);

    EXPECT_CALL(view, displayExceptionalDays(exceptionalDays)).Times(0);

    presenter.onExceptionalDayRemoved(removeCandidate);
}

TEST_F(WorkScheduleEditorPresenterFixture,
       ignores_week_schedule_addition_request_when_it_is_same_as_current)
{
    using namespace dw;
    constexpr Date otherWeekScheduleDate{Year{2000}, Month{3}, Day{4}};
    const std::vector<ui::contracts::WorkScheduleEditor::RoasterRow> expected{
        {Date{Year{1996}, Month{1}, Day{1}}, "10, 11, 12, 13, 14, 0, 0"}};
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    presenter.attachView(view);

    EXPECT_CALL(view, displayRoaster(expected));

    presenter.onWeekScheduleAdded({10, 11, 12, 13, 14, 0, 0},
                                  otherWeekScheduleDate);
}

TEST_F(WorkScheduleEditorPresenterFixture,
       displays_updated_roaster_when_new_week_schedule_added)
{
    using namespace dw;
    constexpr Date otherWeekScheduleDate{Year{2000}, Month{3}, Day{4}};
    const std::vector<ui::contracts::WorkScheduleEditor::RoasterRow> expected{
        {Date{Year{1996}, Month{1}, Day{1}}, "10, 11, 12, 13, 14, 0, 0"},
        {Date{Year{2000}, Month{3}, Day{4}}, "1, 2, 3, 4, 5, 0, 0"}};
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    presenter.attachView(view);

    EXPECT_CALL(view, displayRoaster(expected));
    EXPECT_CALL(view, displayCurrentWeekSchedule(_)).Times(0);
    EXPECT_CALL(view, displayExceptionalDays(_)).Times(0);

    presenter.onWeekScheduleAdded({1, 2, 3, 4, 5, 0, 0}, otherWeekScheduleDate);
}

TEST_F(
    WorkScheduleEditorPresenterFixture,
    displays_updated_roaster_and_current_week_schedule_when_new_week_schedule_removed)
{
    using namespace dw;
    WeekSchedule otherWeekSchedule = buildWeekSchedule({1, 2, 3, 4, 5, 0, 0});
    constexpr Date otherWeekScheduleDate{Year{2000}, Month{3}, Day{4}};
    schedule.addWeekSchedule(otherWeekScheduleDate, otherWeekSchedule);
    const std::vector<ui::contracts::WorkScheduleEditor::RoasterRow> expected{
        {Date{Year{1996}, Month{1}, Day{1}}, "10, 11, 12, 13, 14, 0, 0"}};
    const std::vector<ui::contracts::WorkScheduleEditor::DayAndGoal>
        expectedCurrentWeekSchedule{
            {1, 10}, {2, 11}, {3, 12}, {4, 13}, {5, 14}, {6, 0}, {7, 0}};
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    presenter.attachView(view);

    EXPECT_CALL(view, displayRoaster(expected));
    EXPECT_CALL(view, displayCurrentWeekSchedule(expectedCurrentWeekSchedule));
    EXPECT_CALL(view, displayExceptionalDays(_)).Times(0);

    presenter.onWeekScheduleRemoved(otherWeekScheduleDate);
}

TEST_F(WorkScheduleEditorPresenterFixture, stores_work_schedule_on_confirmation)
{
    using sprint_timer::use_cases::ChangeWorkScheduleCommand;
    using namespace dw;
    auto updatedSchedule = schedule;
    const auto someWeekSchedule = buildWeekSchedule({6, 5, 4, 3, 2, 1, 0});
    constexpr Date someWeekScheduleDate{Year{2020}, Month{8}, Day{3}};
    updatedSchedule.addWeekSchedule(someWeekScheduleDate, someWeekSchedule);
    constexpr Date someHoliday{Year{2020}, Month{9}, Day{1}};
    constexpr Date someExtraDay{Year{2020}, Month{12}, Day{31}};
    updatedSchedule.addExceptionalDay(someHoliday, 0);
    updatedSchedule.addExceptionalDay(someExtraDay, 12);
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    presenter.attachView(view);
    presenter.onWeekScheduleAdded({6, 5, 4, 3, 2, 1, 0}, someWeekScheduleDate);
    presenter.onExceptionalDaysAdded(someHoliday, 1, 0);
    presenter.onExceptionalDaysAdded(someExtraDay, 1, 12);

    EXPECT_CALL(changeWorkScheduleHandler,
                handle(ChangeWorkScheduleCommand{schedule, updatedSchedule}));

    presenter.onScheduleChangeConfirmed();
}

TEST_F(WorkScheduleEditorPresenterFixture,
       does_not_store_work_schedule_if_it_is_not_changed)
{
    const auto updatedSchedule = schedule;
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    presenter.attachView(view);

    EXPECT_CALL(changeWorkScheduleHandler, handle(_)).Times(0);

    presenter.onScheduleChangeConfirmed();
}

TEST_F(WorkScheduleEditorPresenterFixture,
       on_revert_changes_updates_view_with_original_schedule)
{
    using sprint_timer::use_cases::ChangeWorkScheduleCommand;
    using namespace dw;
    auto updatedSchedule = schedule;
    const auto someWeekSchedule = buildWeekSchedule({6, 5, 4, 3, 2, 1, 0});
    constexpr Date someWeekScheduleDate{Year{2020}, Month{8}, Day{3}};
    updatedSchedule.addWeekSchedule(someWeekScheduleDate, someWeekSchedule);
    constexpr Date someHoliday{Year{2020}, Month{9}, Day{1}};
    constexpr Date someExtraDay{Year{2020}, Month{12}, Day{31}};
    updatedSchedule.addExceptionalDay(someHoliday, 0);
    updatedSchedule.addExceptionalDay(someExtraDay, 12);
    const std::vector<ui::contracts::WorkScheduleEditor::DayAndGoal>
        expectedWeekSchedule{
            {1, 10}, {2, 11}, {3, 12}, {4, 13}, {5, 14}, {6, 0}, {7, 0}};
    const std::vector<ui::contracts::WorkScheduleEditor::RoasterRow>
        expectedRoaster{
            {Date{Year{1996}, Month{1}, Day{1}}, "10, 11, 12, 13, 14, 0, 0"}};
    const std::vector<WorkSchedule::DateGoal> expectedDays{
        {{Date{Year{2018}, Month{6}, Day{9}}}, 12},
        {{Date{Year{2018}, Month{6}, Day{11}}}, 0},
        {{Date{Year{2018}, Month{6}, Day{12}}}, 0}};
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    presenter.attachView(view);
    presenter.onWeekScheduleAdded({6, 5, 4, 3, 2, 1, 0}, someWeekScheduleDate);
    presenter.onExceptionalDaysAdded(someHoliday, 1, 0);
    presenter.onExceptionalDaysAdded(someExtraDay, 1, 12);

    EXPECT_CALL(view, displayCurrentWeekSchedule(expectedWeekSchedule));
    EXPECT_CALL(view, displayRoaster(expectedRoaster));
    EXPECT_CALL(view, displayExceptionalDays(expectedDays));

    presenter.onRevertChanges();
}

TEST_F(WorkScheduleEditorPresenterFixture,
       considers_first_day_of_week_when_adding_new_schedule)
{
    using namespace dw;
    const auto oldSchedule = schedule;
    ui::WorkScheduleEditorPresenter sundayFirstPresenter{
        workScheduleHandler, changeWorkScheduleHandler, dw::Weekday::Sunday};
    ON_CALL(workScheduleHandler, handle(_)).WillByDefault(Return(schedule));
    sundayFirstPresenter.attachView(view);
    const auto someWeekSchedule = buildWeekSchedule({7, 6, 5, 4, 3, 2, 1});
    constexpr dw::Date someWeekScheduleDate{Year{2020}, Month{8}, Day{14}};
    sundayFirstPresenter.onWeekScheduleAdded({1, 7, 6, 5, 4, 3, 2},
                                             someWeekScheduleDate);
    schedule.addWeekSchedule(someWeekScheduleDate, someWeekSchedule);

    EXPECT_CALL(
        changeWorkScheduleHandler,
        handle(use_cases::ChangeWorkScheduleCommand{oldSchedule, schedule}));

    sundayFirstPresenter.onScheduleChangeConfirmed();
}
