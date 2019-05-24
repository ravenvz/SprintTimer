#include "gtest/gtest.h"
#include <core/WorkdayTracker.h>

using namespace dw;
using namespace sprint_timer;

WeekSchedule buildSchedule(const std::array<int, 7>& raw_schedule)
{
    WeekSchedule schedule;
    for (size_t i = 0; i < raw_schedule.size(); ++i)
        schedule.setTargetGoal(static_cast<dw::Weekday>(i), raw_schedule[i]);
    return schedule;
}

class WorkdayTrackerFixture : public ::testing::Test {
public:
    const WeekSchedule some_schedule{buildSchedule({5, 5, 5, 5, 5, 0, 0})};
    const dw::Date some_date{Year{1996}, Month{10}, Day{11}};
    WorkdayTracker tracker;
};

TEST_F(WorkdayTrackerFixture, adding_and_removing_exceptional_days)
{
    const DateRange range{Date{Year{2018}, Month{6}, Day{1}},
                          Date{Year{2018}, Month{6}, Day{17}}};
    const int some_goal{22};
    const Date holiday_1{Date{Year{2018}, Month{6}, Day{11}}};
    const Date holiday_2{Date{Year{2018}, Month{6}, Day{12}}};
    const Date extra_workday_1{Date{Year{2018}, Month{6}, Day{9}}};
    WorkdayTracker tracker;
    tracker.addWeekSchedule(some_date, some_schedule);
    tracker.addExceptionalDay(holiday_1, 0);
    tracker.addExceptionalDay(holiday_2, 0);
    tracker.addExceptionalDay(extra_workday_1, some_goal);

    // extra workdays
    EXPECT_TRUE(tracker.isWorkday(extra_workday_1));

    // extra holidays
    EXPECT_FALSE(tracker.isWorkday(holiday_1));
    EXPECT_FALSE(tracker.isWorkday(holiday_2));

    EXPECT_TRUE(tracker.isWorkday(Date{Year{2018}, Month{6}, Day{1}}));
    EXPECT_TRUE(tracker.isWorkday(Date{Year{2018}, Month{6}, Day{15}}));
    EXPECT_FALSE(tracker.isWorkday(Date{Year{2018}, Month{6}, Day{10}}));
    EXPECT_FALSE(tracker.isWorkday(Date{Year{2018}, Month{6}, Day{17}}));
    EXPECT_EQ(10, numWorkdays(tracker, range));

    // Attempting to remove day that is not exceptional does nothing.
    tracker.removeExceptionalDay(Date{Year{2018}, Month{5}, Day{2}});
    EXPECT_EQ(10, numWorkdays(tracker, range));

    tracker.removeExceptionalDay(holiday_1);
    EXPECT_TRUE(tracker.isWorkday(holiday_1));

    tracker.removeExceptionalDay(extra_workday_1);
    EXPECT_FALSE(tracker.isWorkday(extra_workday_1));
}

TEST_F(WorkdayTrackerFixture, removing_exceptional_day)
{
    WorkdayTracker tracker;
    tracker.addWeekSchedule(some_date, some_schedule);
    const dw::Date date_1{Date{Year{2019}, Month{5}, Day{9}}};
    const dw::Date date_2{Date{Year{2019}, Month{5}, Day{22}}};
    tracker.addExceptionalDay(date_1, 0);
    tracker.addExceptionalDay(date_2, 10);

    tracker.removeExceptionalDay(date_1);
    tracker.removeExceptionalDay(date_2);
}

TEST_F(WorkdayTrackerFixture,
       no_extra_day_can_be_workday_and_holiday_simultaneously)
{
    WorkdayTracker tracker;
    tracker.addWeekSchedule(some_date, some_schedule);
    const int some_goal{22};

    tracker.addExceptionalDay(Date{Year{2019}, Month{3}, Day{8}}, 0);
    tracker.addExceptionalDay(Date{Year{2019}, Month{3}, Day{8}}, some_goal);

    tracker.addExceptionalDay(Date{Year{2019}, Month{5}, Day{9}}, some_goal);
    tracker.addExceptionalDay(Date{Year{2019}, Month{5}, Day{9}}, 0);

    // Last inserted day overrides previously inserted on conflict

    EXPECT_TRUE(tracker.isWorkday(Date{Year{2019}, Month{3}, Day{8}}));
    EXPECT_FALSE(tracker.isWorkday(Date{Year{2019}, Month{5}, Day{9}}));
}

TEST_F(WorkdayTrackerFixture,
       treats_all_days_that_are_before_first_scheduled_date_as_holidays)
{
    tracker.addWeekSchedule(Date{Year{2019}, Month{1}, Day{4}}, some_schedule);

    EXPECT_FALSE(tracker.isWorkday(Date{Year{2019}, Month{1}, Day{3}}));
    EXPECT_TRUE(tracker.isWorkday(Date{Year{2019}, Month{1}, Day{4}}));
}

TEST_F(WorkdayTrackerFixture,
       overwrites_last_schedule_when_new_is_inserted_with_same_date)
{
    const WeekSchedule newSchedule{buildSchedule({5, 5, 5, 5, 5, 0, 5})};
    WorkdayTracker::ScheduleRoaster expected{{some_date, newSchedule}};

    tracker.addWeekSchedule(some_date, some_schedule);
    tracker.addWeekSchedule(some_date, newSchedule);

    EXPECT_EQ(expected, tracker.scheduleRoaster());
}

TEST_F(WorkdayTrackerFixture, returns_schedule_that_is_active_now)
{
    const WeekSchedule expected{some_schedule};
    const WeekSchedule old_schedule{buildSchedule({3, 3, 3, 3, 3, 3, 3})};
    tracker.addWeekSchedule(some_date, some_schedule);
    tracker.addWeekSchedule(some_date - Years{1}, old_schedule);

    EXPECT_EQ(expected, tracker.currentSchedule());
}

TEST_F(WorkdayTrackerFixture, returns_schedule_for_specified_date)
{
    const WeekSchedule empty_schedule;
    const WeekSchedule first_schedule{buildSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule second_schedule{buildSchedule({2, 2, 2, 2, 2, 2, 2})};
    const WeekSchedule third_schedule{buildSchedule({3, 3, 3, 3, 3, 3, 3})};
    const auto date_1 = some_date;
    const auto date_2 = some_date + Years{1};
    const auto date_3 = some_date + Years{2};

    tracker.addWeekSchedule(date_1, first_schedule);
    tracker.addWeekSchedule(date_2, second_schedule);
    tracker.addWeekSchedule(date_3, third_schedule);

    EXPECT_EQ(empty_schedule, tracker.scheduleFor(date_1 - Days{1}));
    EXPECT_EQ(first_schedule, tracker.scheduleFor(date_1));
    EXPECT_EQ(first_schedule, tracker.scheduleFor(date_1 + Days{1}));
    EXPECT_EQ(first_schedule, tracker.scheduleFor(date_2 - Days{1}));
    EXPECT_EQ(second_schedule, tracker.scheduleFor(date_2));
    EXPECT_EQ(second_schedule, tracker.scheduleFor(date_2 + Days{1}));
    EXPECT_EQ(second_schedule, tracker.scheduleFor(date_3 - Days{1}));
    EXPECT_EQ(third_schedule, tracker.scheduleFor(date_3));
    EXPECT_EQ(third_schedule, tracker.scheduleFor(date_3 + Days{1}));
}

TEST_F(WorkdayTrackerFixture,
       ignores_schedule_insertion_when_current_insertion_has_same_schedule)
{
    const WeekSchedule schedule{buildSchedule({5, 5, 5, 5, 5, 5, 5})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    WorkdayTracker::ScheduleRoaster expected{{date_1, schedule}};

    tracker.addWeekSchedule(date_1, schedule);
    tracker.addWeekSchedule(date_2, schedule);

    EXPECT_EQ(expected, tracker.scheduleRoaster());
}

TEST_F(WorkdayTrackerFixture,
       ignores_schedule_insertion_when_it_is_same_as_schedule_for_previous_date)
{
    const WeekSchedule schedule_1{buildSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule schedule_2{buildSchedule({2, 2, 2, 2, 2, 2, 2})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    const auto date_between = date_1 + Months{4};
    const auto date_ahead = date_1 + Years{2};
    tracker.addWeekSchedule(date_1, schedule_1);
    tracker.addWeekSchedule(date_2, schedule_2);
    WorkdayTracker::ScheduleRoaster expected{{date_1, schedule_1},
                                             {date_2, schedule_2}};

    tracker.addWeekSchedule(date_between, schedule_1);
    tracker.addWeekSchedule(date_ahead, schedule_2);

    EXPECT_EQ(expected, tracker.scheduleRoaster());
}

TEST_F(
    WorkdayTrackerFixture,
    merges_schedules_when_schedule_for_first_date_that_is_greater_than_inserted_is_same)
{
    const WeekSchedule schedule_1{buildSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule schedule_2{buildSchedule({2, 2, 2, 2, 2, 2, 2})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    const auto date_between = date_1 + Months{4};
    tracker.addWeekSchedule(date_1, schedule_1);
    tracker.addWeekSchedule(date_2, schedule_2);
    WorkdayTracker::ScheduleRoaster expected{{date_1, schedule_1},
                                             {date_between, schedule_2}};

    tracker.addWeekSchedule(date_between, schedule_2);

    EXPECT_EQ(expected, tracker.scheduleRoaster());
}

TEST_F(
    WorkdayTrackerFixture,
    removing_schedule_for_specific_date_is_ignored_when_there_is_no_schedule_for_that_date)
{
    const WeekSchedule schedule_1{buildSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule schedule_2{buildSchedule({2, 2, 2, 2, 2, 2, 2})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    const auto date_between = date_1 + Months{4};
    tracker.addWeekSchedule(date_1, schedule_1);
    tracker.addWeekSchedule(date_2, schedule_2);
    WorkdayTracker::ScheduleRoaster expected{{date_1, schedule_1},
                                             {date_2, schedule_2}};

    tracker.removeWeekSchedule(date_between);

    EXPECT_EQ(expected, tracker.scheduleRoaster());
}

TEST_F(WorkdayTrackerFixture, removing_schedule_that_is_earliest_one)
{
    const WeekSchedule schedule_1{buildSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule schedule_2{buildSchedule({2, 2, 2, 2, 2, 2, 2})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    tracker.addWeekSchedule(date_1, schedule_1);
    tracker.addWeekSchedule(date_2, schedule_2);
    WorkdayTracker::ScheduleRoaster expected{{date_2, schedule_2}};

    tracker.removeWeekSchedule(some_date);

    EXPECT_EQ(expected, tracker.scheduleRoaster());
}

TEST_F(WorkdayTrackerFixture, removing_schedule_that_is_latest_one)
{
    const WeekSchedule schedule_1{buildSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule schedule_2{buildSchedule({2, 2, 2, 2, 2, 2, 2})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    tracker.addWeekSchedule(date_1, schedule_1);
    tracker.addWeekSchedule(date_2, schedule_2);
    WorkdayTracker::ScheduleRoaster expected{{date_1, schedule_1}};

    tracker.removeWeekSchedule(date_2);

    EXPECT_EQ(expected, tracker.scheduleRoaster());
}

TEST_F(WorkdayTrackerFixture, removing_schedule_in_the_middle)
{
    const WeekSchedule schedule_1{buildSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule schedule_2{buildSchedule({2, 2, 2, 2, 2, 2, 2})};
    const WeekSchedule schedule_3{buildSchedule({3, 3, 3, 3, 3, 3, 3})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    const auto date_3 = date_1 + Years{2};
    tracker.addWeekSchedule(date_1, schedule_1);
    tracker.addWeekSchedule(date_2, schedule_2);
    tracker.addWeekSchedule(date_3, schedule_3);
    WorkdayTracker::ScheduleRoaster expected{{date_1, schedule_1},
                                             {date_3, schedule_3}};

    tracker.removeWeekSchedule(date_2);

    EXPECT_EQ(expected, tracker.scheduleRoaster());
}

TEST_F(WorkdayTrackerFixture,
       removing_schedule_in_the_middle_merges_adjacent_if_they_are_same)
{
    const WeekSchedule schedule_1{buildSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule schedule_2{buildSchedule({2, 2, 2, 2, 2, 2, 2})};
    const WeekSchedule schedule_3{buildSchedule({1, 1, 1, 1, 1, 1, 1})};
    const auto date_1 = some_date;
    const auto date_2 = date_1 + Years{1};
    const auto date_3 = date_1 + Years{2};
    tracker.addWeekSchedule(date_1, schedule_1);
    tracker.addWeekSchedule(date_2, schedule_2);
    tracker.addWeekSchedule(date_3, schedule_3);
    WorkdayTracker::ScheduleRoaster expected{{date_1, schedule_1}};

    tracker.removeWeekSchedule(date_2);

    EXPECT_EQ(expected, tracker.scheduleRoaster());
}

TEST_F(WorkdayTrackerFixture, returns_estimated_goal_for_given_period)
{
    const WeekSchedule schedule{buildSchedule({13, 13, 13, 13, 13, 0, 0})};
    tracker.addWeekSchedule(Date{Year{2019}, Month{5}, Day{1}}, schedule);
    tracker.addExceptionalDay(Date{Year{2019}, Month{5}, Day{1}}, 0);
    tracker.addExceptionalDay(Date{Year{2019}, Month{5}, Day{2}}, 0);
    tracker.addExceptionalDay(Date{Year{2019}, Month{5}, Day{3}}, 0);
    tracker.addExceptionalDay(Date{Year{2019}, Month{5}, Day{9}}, 0);
    tracker.addExceptionalDay(Date{Year{2019}, Month{5}, Day{10}}, 0);
    const int expected{234};
    const DateRange range{Date{Year{2019}, Month{5}, Day{1}},
                          Date{Year{2019}, Month{5}, Day{31}}};

    const int actual = goalFor(tracker, range);

    EXPECT_EQ(expected, actual);
}

TEST_F(WorkdayTrackerFixture, schedule_benchmark)
{
    tracker.addWeekSchedule(some_date, buildSchedule({1, 1, 1, 1, 1, 1, 1}));
    tracker.addWeekSchedule(Date{Year{1997}, Month{1}, Day{1}},
                            buildSchedule({2, 2, 2, 2, 2, 2, 2}));
    tracker.addWeekSchedule(Date{Year{1998}, Month{1}, Day{1}},
                            buildSchedule({3, 3, 3, 3, 3, 3, 3}));
    tracker.addWeekSchedule(Date{Year{1999}, Month{1}, Day{1}},
                            buildSchedule({4, 4, 4, 4, 4, 4, 4}));
    tracker.addWeekSchedule(Date{Year{2000}, Month{1}, Day{1}},
                            buildSchedule({5, 5, 5, 5, 5, 5, 5}));
    tracker.addWeekSchedule(Date{Year{2001}, Month{1}, Day{1}},
                            buildSchedule({6, 6, 6, 6, 6, 6, 6}));
    tracker.addWeekSchedule(Date{Year{2002}, Month{1}, Day{1}},
                            buildSchedule({7, 7, 7, 7, 7, 7, 7}));
    tracker.addWeekSchedule(Date{Year{2004}, Month{1}, Day{1}},
                            buildSchedule({8, 8, 8, 8, 8, 8, 8}));
    tracker.addWeekSchedule(Date{Year{2005}, Month{1}, Day{1}},
                            buildSchedule({9, 9, 9, 9, 9, 9, 9}));
    tracker.addWeekSchedule(Date{Year{2006}, Month{1}, Day{1}},
                            buildSchedule({10, 10, 10, 10, 10, 10, 10}));

    const dw::Date stop_date{some_date + dw::Years{200}};
    for (dw::Date date = some_date; date < stop_date;
         date = date + dw::Days{1}) {
        EXPECT_TRUE(tracker.isWorkday(date));
    }
}
