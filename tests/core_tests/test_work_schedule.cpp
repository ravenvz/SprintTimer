#include "gtest/gtest.h"
#include <core/WorkSchedule.h>

using namespace dw;
using namespace sprint_timer;

WeekSchedule buildWeekSchedule(const std::array<int, 7>& raw_schedule)
{
    WeekSchedule w_schedule;
    for (size_t i = 0; i < raw_schedule.size(); ++i)
        w_schedule.setTargetGoal(static_cast<dw::Weekday>(i), raw_schedule[i]);
    return w_schedule;
}

class WorkScheduleFixture : public ::testing::Test {
public:
    const WeekSchedule some_week_schedule{
        buildWeekSchedule({5, 5, 5, 5, 5, 0, 0})};
    const dw::Date some_date{Year{1996}, Month{10}, Day{11}};
    WorkSchedule workSchedule;
};

TEST_F(WorkScheduleFixture, adding_and_removing_exceptional_days)
{
    const DateRange range{Date{Year{2018}, Month{6}, Day{1}},
                          Date{Year{2018}, Month{6}, Day{17}}};
    const int some_goal{22};
    const Date holiday_1{Date{Year{2018}, Month{6}, Day{11}}};
    const Date holiday_2{Date{Year{2018}, Month{6}, Day{12}}};
    const Date extra_workday_1{Date{Year{2018}, Month{6}, Day{9}}};
    WorkSchedule workSchedule;
    workSchedule.addWeekSchedule(some_date, some_week_schedule);
    workSchedule.addExceptionalDay(holiday_1, 0);
    workSchedule.addExceptionalDay(holiday_2, 0);
    workSchedule.addExceptionalDay(extra_workday_1, some_goal);

    // extra workdays
    EXPECT_TRUE(workSchedule.isWorkday(extra_workday_1));

    // extra holidays
    EXPECT_FALSE(workSchedule.isWorkday(holiday_1));
    EXPECT_FALSE(workSchedule.isWorkday(holiday_2));

    EXPECT_TRUE(workSchedule.isWorkday(Date{Year{2018}, Month{6}, Day{1}}));
    EXPECT_TRUE(workSchedule.isWorkday(Date{Year{2018}, Month{6}, Day{15}}));
    EXPECT_FALSE(workSchedule.isWorkday(Date{Year{2018}, Month{6}, Day{10}}));
    EXPECT_FALSE(workSchedule.isWorkday(Date{Year{2018}, Month{6}, Day{17}}));
    EXPECT_EQ(10, numWorkdays(workSchedule, range));

    // Attempting to remove day that is not exceptional does nothing.
    workSchedule.removeExceptionalDay(Date{Year{2018}, Month{5}, Day{2}});
    EXPECT_EQ(10, numWorkdays(workSchedule, range));

    workSchedule.removeExceptionalDay(holiday_1);
    EXPECT_TRUE(workSchedule.isWorkday(holiday_1));

    workSchedule.removeExceptionalDay(extra_workday_1);
    EXPECT_FALSE(workSchedule.isWorkday(extra_workday_1));
}

TEST_F(WorkScheduleFixture, removing_exceptional_day)
{
    WorkSchedule workSchedule;
    workSchedule.addWeekSchedule(some_date, some_week_schedule);
    const dw::Date date_1{Date{Year{2019}, Month{5}, Day{9}}};
    const dw::Date date_2{Date{Year{2019}, Month{5}, Day{22}}};
    workSchedule.addExceptionalDay(date_1, 0);
    workSchedule.addExceptionalDay(date_2, 10);

    workSchedule.removeExceptionalDay(date_1);
    workSchedule.removeExceptionalDay(date_2);
}

TEST_F(WorkScheduleFixture,
       no_extra_day_can_be_workday_and_holiday_simultaneously)
{
    WorkSchedule workSchedule;
    workSchedule.addWeekSchedule(some_date, some_week_schedule);
    const int some_goal{22};

    workSchedule.addExceptionalDay(Date{Year{2019}, Month{3}, Day{8}}, 0);
    workSchedule.addExceptionalDay(Date{Year{2019}, Month{3}, Day{8}},
                                   some_goal);

    workSchedule.addExceptionalDay(Date{Year{2019}, Month{5}, Day{9}},
                                   some_goal);
    workSchedule.addExceptionalDay(Date{Year{2019}, Month{5}, Day{9}}, 0);

    // Last inserted day overrides previously inserted on conflict

    EXPECT_TRUE(workSchedule.isWorkday(Date{Year{2019}, Month{3}, Day{8}}));
    EXPECT_FALSE(workSchedule.isWorkday(Date{Year{2019}, Month{5}, Day{9}}));
}

TEST_F(WorkScheduleFixture,
       treats_all_days_that_are_before_first_scheduled_date_as_holidays)
{
    workSchedule.addWeekSchedule(Date{Year{2019}, Month{1}, Day{4}},
                                 some_week_schedule);

    EXPECT_FALSE(workSchedule.isWorkday(Date{Year{2019}, Month{1}, Day{3}}));
    EXPECT_TRUE(workSchedule.isWorkday(Date{Year{2019}, Month{1}, Day{4}}));
}

TEST_F(WorkScheduleFixture,
       overwrites_last_schedule_when_new_is_inserted_with_same_date)
{
    const WeekSchedule newSchedule{buildWeekSchedule({5, 5, 5, 5, 5, 0, 5})};
    WorkSchedule::Roaster expected{{some_date, newSchedule}};

    workSchedule.addWeekSchedule(some_date, some_week_schedule);
    workSchedule.addWeekSchedule(some_date, newSchedule);

    EXPECT_EQ(expected, workSchedule.roaster());
}

TEST_F(WorkScheduleFixture,
       returns_empty_week_schedule_as_active_schedule_when_there_is_no_schedule)
{
    const WeekSchedule expected;

    EXPECT_EQ(expected, workSchedule.currentWeekSchedule());
}

TEST_F(WorkScheduleFixture, returns_week_schedule_for_current_date)
{
    const WeekSchedule expected{some_week_schedule};
    const WeekSchedule old_schedule{buildWeekSchedule({3, 3, 3, 3, 3, 3, 3})};
    workSchedule.addWeekSchedule(some_date, some_week_schedule);
    workSchedule.addWeekSchedule(some_date - Years{1}, old_schedule);

    EXPECT_EQ(expected, workSchedule.currentWeekSchedule());
}

TEST_F(WorkScheduleFixture, returns_week_schedule_for_specified_date)
{
    const WeekSchedule empty_schedule;
    const WeekSchedule first_schedule{buildWeekSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule second_schedule{
        buildWeekSchedule({2, 2, 2, 2, 2, 2, 2})};
    const WeekSchedule third_schedule{buildWeekSchedule({3, 3, 3, 3, 3, 3, 3})};
    const auto date_1 = some_date;
    const auto date_2 = some_date + Years{1};
    const auto date_3 = some_date + Years{2};

    workSchedule.addWeekSchedule(date_1, first_schedule);
    workSchedule.addWeekSchedule(date_2, second_schedule);
    workSchedule.addWeekSchedule(date_3, third_schedule);

    EXPECT_EQ(empty_schedule, workSchedule.weekScheduleFor(date_1 - Days{1}));
    EXPECT_EQ(first_schedule, workSchedule.weekScheduleFor(date_1));
    EXPECT_EQ(first_schedule, workSchedule.weekScheduleFor(date_1 + Days{1}));
    EXPECT_EQ(first_schedule, workSchedule.weekScheduleFor(date_2 - Days{1}));
    EXPECT_EQ(second_schedule, workSchedule.weekScheduleFor(date_2));
    EXPECT_EQ(second_schedule, workSchedule.weekScheduleFor(date_2 + Days{1}));
    EXPECT_EQ(second_schedule, workSchedule.weekScheduleFor(date_3 - Days{1}));
    EXPECT_EQ(third_schedule, workSchedule.weekScheduleFor(date_3));
    EXPECT_EQ(third_schedule, workSchedule.weekScheduleFor(date_3 + Days{1}));
}

TEST_F(WorkScheduleFixture,
       ignores_week_schedule_insertion_when_current_insertion_has_same_schedule)
{
    const WeekSchedule w_schedule{buildWeekSchedule({5, 5, 5, 5, 5, 5, 5})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    WorkSchedule::Roaster expected{{date_1, w_schedule}};

    workSchedule.addWeekSchedule(date_1, w_schedule);
    workSchedule.addWeekSchedule(date_2, w_schedule);

    EXPECT_EQ(expected, workSchedule.roaster());
}

TEST_F(
    WorkScheduleFixture,
    ignores_week_schedule_insertion_when_it_is_same_as_schedule_for_previous_date)
{
    const WeekSchedule w_schedule_1{buildWeekSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule w_schedule_2{buildWeekSchedule({2, 2, 2, 2, 2, 2, 2})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    const auto date_between = date_1 + Months{4};
    const auto date_ahead = date_1 + Years{2};
    workSchedule.addWeekSchedule(date_1, w_schedule_1);
    workSchedule.addWeekSchedule(date_2, w_schedule_2);
    WorkSchedule::Roaster expected{{date_1, w_schedule_1},
                                   {date_2, w_schedule_2}};

    workSchedule.addWeekSchedule(date_between, w_schedule_1);
    workSchedule.addWeekSchedule(date_ahead, w_schedule_2);

    EXPECT_EQ(expected, workSchedule.roaster());
}

TEST_F(
    WorkScheduleFixture,
    merges_week_schedules_when_schedule_for_first_date_that_is_greater_than_inserted_is_same)
{
    const WeekSchedule w_schedule_1{buildWeekSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule w_schedule_2{buildWeekSchedule({2, 2, 2, 2, 2, 2, 2})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    const auto date_between = date_1 + Months{4};
    workSchedule.addWeekSchedule(date_1, w_schedule_1);
    workSchedule.addWeekSchedule(date_2, w_schedule_2);
    WorkSchedule::Roaster expected{{date_1, w_schedule_1},
                                   {date_between, w_schedule_2}};

    workSchedule.addWeekSchedule(date_between, w_schedule_2);

    EXPECT_EQ(expected, workSchedule.roaster());
}

TEST_F(
    WorkScheduleFixture,
    removing_week_schedule_for_specific_date_is_ignored_when_there_is_no_schedule_for_that_date)
{
    const WeekSchedule w_schedule_1{buildWeekSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule w_schedule_2{buildWeekSchedule({2, 2, 2, 2, 2, 2, 2})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    const auto date_between = date_1 + Months{4};
    workSchedule.addWeekSchedule(date_1, w_schedule_1);
    workSchedule.addWeekSchedule(date_2, w_schedule_2);
    WorkSchedule::Roaster expected{{date_1, w_schedule_1},
                                   {date_2, w_schedule_2}};

    workSchedule.removeWeekSchedule(date_between);

    EXPECT_EQ(expected, workSchedule.roaster());
}

TEST_F(WorkScheduleFixture, removing_week_schedule_that_is_earliest_one)
{
    const WeekSchedule w_schedule_1{buildWeekSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule w_schedule_2{buildWeekSchedule({2, 2, 2, 2, 2, 2, 2})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    workSchedule.addWeekSchedule(date_1, w_schedule_1);
    workSchedule.addWeekSchedule(date_2, w_schedule_2);
    WorkSchedule::Roaster expected{{date_2, w_schedule_2}};

    workSchedule.removeWeekSchedule(some_date);

    EXPECT_EQ(expected, workSchedule.roaster());
}

TEST_F(WorkScheduleFixture, removing_week_schedule_that_is_latest_one)
{
    const WeekSchedule w_schedule_1{buildWeekSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule w_schedule_2{buildWeekSchedule({2, 2, 2, 2, 2, 2, 2})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    workSchedule.addWeekSchedule(date_1, w_schedule_1);
    workSchedule.addWeekSchedule(date_2, w_schedule_2);
    WorkSchedule::Roaster expected{{date_1, w_schedule_1}};

    workSchedule.removeWeekSchedule(date_2);

    EXPECT_EQ(expected, workSchedule.roaster());
}

TEST_F(WorkScheduleFixture, removing_week_schedule_in_the_middle)
{
    const WeekSchedule w_schedule_1{buildWeekSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule w_schedule_2{buildWeekSchedule({2, 2, 2, 2, 2, 2, 2})};
    const WeekSchedule w_schedule_3{buildWeekSchedule({3, 3, 3, 3, 3, 3, 3})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    const auto date_3 = date_1 + Years{2};
    workSchedule.addWeekSchedule(date_1, w_schedule_1);
    workSchedule.addWeekSchedule(date_2, w_schedule_2);
    workSchedule.addWeekSchedule(date_3, w_schedule_3);
    WorkSchedule::Roaster expected{{date_1, w_schedule_1},
                                   {date_3, w_schedule_3}};

    workSchedule.removeWeekSchedule(date_2);

    EXPECT_EQ(expected, workSchedule.roaster());
}

TEST_F(WorkScheduleFixture,
       removing_week_schedule_in_the_middle_merges_adjacent_if_they_are_same)
{
    const WeekSchedule w_schedule_1{buildWeekSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule w_schedule_2{buildWeekSchedule({2, 2, 2, 2, 2, 2, 2})};
    const WeekSchedule w_schedule_3{buildWeekSchedule({1, 1, 1, 1, 1, 1, 1})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    const auto date_3 = date_1 + Years{2};
    workSchedule.addWeekSchedule(date_1, w_schedule_1);
    workSchedule.addWeekSchedule(date_2, w_schedule_2);
    workSchedule.addWeekSchedule(date_3, w_schedule_3);
    WorkSchedule::Roaster expected{{date_1, w_schedule_1}};

    workSchedule.removeWeekSchedule(date_2);

    EXPECT_EQ(expected, workSchedule.roaster());
}

TEST_F(WorkScheduleFixture, returns_estimated_goal_for_given_period)
{
    const WeekSchedule w_schedule{
        buildWeekSchedule({13, 13, 13, 13, 13, 0, 0})};
    workSchedule.addWeekSchedule(Date{Year{2019}, Month{5}, Day{1}},
                                 w_schedule);
    workSchedule.addExceptionalDay(Date{Year{2019}, Month{5}, Day{1}}, 0);
    workSchedule.addExceptionalDay(Date{Year{2019}, Month{5}, Day{2}}, 0);
    workSchedule.addExceptionalDay(Date{Year{2019}, Month{5}, Day{3}}, 0);
    workSchedule.addExceptionalDay(Date{Year{2019}, Month{5}, Day{9}}, 0);
    workSchedule.addExceptionalDay(Date{Year{2019}, Month{5}, Day{10}}, 0);
    const int expected{234};
    const DateRange range{Date{Year{2019}, Month{5}, Day{1}},
                          Date{Year{2019}, Month{5}, Day{31}}};

    const int actual = goalFor(workSchedule, range);

    EXPECT_EQ(expected, actual);
}

TEST_F(WorkScheduleFixture, w_schedule_benchmark)
{
    workSchedule.addWeekSchedule(some_date,
                                 buildWeekSchedule({1, 1, 1, 1, 1, 1, 1}));
    workSchedule.addWeekSchedule(Date{Year{1997}, Month{1}, Day{1}},
                                 buildWeekSchedule({2, 2, 2, 2, 2, 2, 2}));
    workSchedule.addWeekSchedule(Date{Year{1998}, Month{1}, Day{1}},
                                 buildWeekSchedule({3, 3, 3, 3, 3, 3, 3}));
    workSchedule.addWeekSchedule(Date{Year{1999}, Month{1}, Day{1}},
                                 buildWeekSchedule({4, 4, 4, 4, 4, 4, 4}));
    workSchedule.addWeekSchedule(Date{Year{2000}, Month{1}, Day{1}},
                                 buildWeekSchedule({5, 5, 5, 5, 5, 5, 5}));
    workSchedule.addWeekSchedule(Date{Year{2001}, Month{1}, Day{1}},
                                 buildWeekSchedule({6, 6, 6, 6, 6, 6, 6}));
    workSchedule.addWeekSchedule(Date{Year{2002}, Month{1}, Day{1}},
                                 buildWeekSchedule({7, 7, 7, 7, 7, 7, 7}));
    workSchedule.addWeekSchedule(Date{Year{2004}, Month{1}, Day{1}},
                                 buildWeekSchedule({8, 8, 8, 8, 8, 8, 8}));
    workSchedule.addWeekSchedule(Date{Year{2005}, Month{1}, Day{1}},
                                 buildWeekSchedule({9, 9, 9, 9, 9, 9, 9}));
    workSchedule.addWeekSchedule(
        Date{Year{2006}, Month{1}, Day{1}},
        buildWeekSchedule({10, 10, 10, 10, 10, 10, 10}));

    const dw::Date stop_date{some_date + dw::Years{200}};
    for (dw::Date date = some_date; date < stop_date;
         date = date + dw::Days{1}) {
        EXPECT_TRUE(workSchedule.isWorkday(date));
    }
}
