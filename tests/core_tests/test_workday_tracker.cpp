#include "gtest/gtest.h"
#include <core/WorkdayTracker.h>

using namespace dw;
using namespace sprint_timer;
using namespace sprint_timer::utils;

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

TEST_F(WorkdayTrackerFixture, trivial_test)
{
    const DateRange range{Date{Year{2018}, Month{6}, Day{1}},
                          Date{Year{2018}, Month{6}, Day{17}}};
    const int some_goal{22};
    WorkdayTracker tracker;
    tracker.addWeekSchedule(some_date, some_schedule);
    tracker.addExtraHoliday(Date{Year{2018}, Month{6}, Day{11}});
    tracker.addExtraHoliday(Date{Year{2018}, Month{6}, Day{12}});
    tracker.addExtraWorkday(Date{Year{2018}, Month{6}, Day{9}}, some_goal);

    // extra workdays
    EXPECT_TRUE(tracker.isWorkday(Date{Year{2018}, Month{6}, Day{9}}));

    // extra holidays
    EXPECT_FALSE(tracker.isWorkday(Date{Year{2018}, Month{6}, Day{11}}));
    EXPECT_FALSE(tracker.isWorkday(Date{Year{2018}, Month{6}, Day{12}}));

    EXPECT_TRUE(tracker.isWorkday(Date{Year{2018}, Month{6}, Day{1}}));
    EXPECT_TRUE(tracker.isWorkday(Date{Year{2018}, Month{6}, Day{15}}));
    EXPECT_FALSE(tracker.isWorkday(Date{Year{2018}, Month{6}, Day{10}}));
    EXPECT_FALSE(tracker.isWorkday(Date{Year{2018}, Month{6}, Day{17}}));
    EXPECT_EQ(10, numWorkdays(tracker, range));
}

TEST_F(WorkdayTrackerFixture,
       no_extra_day_can_be_workday_and_holiday_simultaneously)
{
    WorkdayTracker tracker;
    tracker.addWeekSchedule(some_date, some_schedule);
    const int some_goal{22};

    tracker.addExtraHoliday(Date{Year{2019}, Month{3}, Day{8}});
    tracker.addExtraWorkday(Date{Year{2019}, Month{3}, Day{8}}, some_goal);

    tracker.addExtraWorkday(Date{Year{2019}, Month{5}, Day{9}}, some_goal);
    tracker.addExtraHoliday(Date{Year{2019}, Month{5}, Day{9}});

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

    std::cout << tracker << std::endl;
    EXPECT_EQ(expected, tracker.currentSchedule());
}

TEST_F(WorkdayTrackerFixture, returns_schedule_for_specified_date)
{
    const WeekSchedule empty_schedule;
    const WeekSchedule first_schedule{buildSchedule({1, 1, 1, 1, 1, 1, 1})};
    const WeekSchedule second_schedule{buildSchedule({2, 2, 2, 2, 2, 2, 2})};
    const WeekSchedule third_schedule{buildSchedule({3, 3, 3, 3, 3, 3, 3})};

    tracker.addWeekSchedule(some_date, first_schedule);
    tracker.addWeekSchedule(some_date + Years{1}, second_schedule);
    tracker.addWeekSchedule(some_date + Years{2}, third_schedule);

    EXPECT_EQ(empty_schedule, tracker.scheduleFor(some_date - Days{1}));
    EXPECT_EQ(first_schedule, tracker.scheduleFor(some_date));
    EXPECT_EQ(first_schedule, tracker.scheduleFor(some_date + Days{1}));
    EXPECT_EQ(first_schedule,
              tracker.scheduleFor(some_date + Years{1} - Days{1}));
    EXPECT_EQ(second_schedule, tracker.scheduleFor(some_date + Years{1}));
    EXPECT_EQ(second_schedule,
              tracker.scheduleFor(some_date + Years{1} + Days{1}));
    EXPECT_EQ(second_schedule,
              tracker.scheduleFor(some_date + Years{2} - Days{1}));
    EXPECT_EQ(third_schedule, tracker.scheduleFor(some_date + Years{2}));
    EXPECT_EQ(third_schedule,
              tracker.scheduleFor(some_date + Years{2} + Days{1}));
}

TEST_F(WorkdayTrackerFixture,
       ignores_schedule_insertion_when_current_insertion_has_same_schedule)
{
    const WeekSchedule schedule{buildSchedule({5, 5, 5, 5, 5, 5, 5})};
    WorkdayTracker::ScheduleRoaster expected{{some_date, schedule}};

    tracker.addWeekSchedule(some_date, schedule);
    tracker.addWeekSchedule(some_date + Years{1}, schedule);

    EXPECT_EQ(expected, tracker.scheduleRoaster());
}

TEST_F(WorkdayTrackerFixture, schedule_benchmark) {
    tracker.addWeekSchedule(some_date, buildSchedule({1, 1, 1, 1, 1, 1, 1}));
    tracker.addWeekSchedule(Date{Year{1997}, Month{1}, Day{1}}, buildSchedule({2, 2, 2, 2, 2, 2, 2}));
    tracker.addWeekSchedule(Date{Year{1998}, Month{1}, Day{1}}, buildSchedule({3, 3, 3, 3, 3, 3, 3}));
    tracker.addWeekSchedule(Date{Year{1999}, Month{1}, Day{1}}, buildSchedule({4, 4, 4, 4, 4, 4, 4}));
    tracker.addWeekSchedule(Date{Year{2000}, Month{1}, Day{1}}, buildSchedule({5, 5, 5, 5, 5, 5, 5}));
    tracker.addWeekSchedule(Date{Year{2001}, Month{1}, Day{1}}, buildSchedule({6, 6, 6, 6, 6, 6, 6}));
    tracker.addWeekSchedule(Date{Year{2002}, Month{1}, Day{1}}, buildSchedule({7, 7, 7, 7, 7, 7, 7}));
    tracker.addWeekSchedule(Date{Year{2004}, Month{1}, Day{1}}, buildSchedule({8, 8, 8, 8, 8, 8, 8}));
    tracker.addWeekSchedule(Date{Year{2005}, Month{1}, Day{1}}, buildSchedule({9, 9, 9, 9, 9, 9, 9}));
    tracker.addWeekSchedule(Date{Year{2006}, Month{1}, Day{1}}, buildSchedule({10, 10, 10, 10, 10, 10, 10}));

    const dw::Date stop_date{some_date + dw::Years{200}};
    for (dw::Date date = some_date; date < stop_date; date = date + dw::Days{1}) {
        EXPECT_TRUE(tracker.isWorkday(date));
    }

}
