#include "gtest/gtest.h"
#include <core/WorkdayTracker.h>

class WorkdayTrackerFixture : public ::testing::Test {
public:
    sprint_timer::utils::WeekdaySelection selection{
        31}; // Monday - Friday selected
};

TEST_F(WorkdayTrackerFixture, test_name)
{
    using namespace dw;
    using namespace sprint_timer;
    const DateRange range{Date{Year{2018}, Month{6}, Day{1}},
                          Date{Year{2018}, Month{6}, Day{17}}};
    WorkdayTracker tracker{selection};
    tracker.addExtraHoliday(Date{Year{2018}, Month{6}, Day{11}});
    tracker.addExtraHoliday(Date{Year{2018}, Month{6}, Day{12}});
    tracker.addExtraWorkday(Date{Year{2018}, Month{6}, Day{9}});

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
