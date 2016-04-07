#include "core/timeinterval.h"
#include <TestHarness.h>


TEST_GROUP(TimeInterval){

};

TEST(TimeInterval, test_size_in_days_should_be_at_least_one)
{
    DateTime dt = DateTime::fromYMD(2015, 8, 3);
    TimeInterval interval{dt, dt};
    CHECK_EQUAL(1, interval.sizeInDays());
}

TEST(TimeInterval, test_returns_size_in_days)
{
    TimeInterval interval{DateTime::fromYMD(2015, 8, 3),
                          DateTime::fromYMD(2015, 8, 4)};
    CHECK_EQUAL(2, interval.sizeInDays());
}

TEST(TimeInterval, test_returns_size_in_days_from_incorrect_interval)
{
    TimeInterval interval{DateTime::fromYMD(2015, 8, 18),
                          DateTime::fromYMD(2015, 8, 3)};
    CHECK_EQUAL(16, interval.sizeInDays());
}

TEST(TimeInterval, test_returns_correct_date_diff_between_intervals_start)
{
    TimeInterval first{DateTime::fromYMD(2015, 8, 3),
                       DateTime::fromYMD(2015, 8, 31)};
    TimeInterval second{DateTime::fromYMD(2015, 8, 18),
                        DateTime::fromYMD(2015, 8, 20)};
    CHECK_EQUAL(15, startDateAbsDiff(first, second));
}

TEST(TimeInterval,
     test_returns_correct_date_diff_between_incorrect_intervals_start)
{
    TimeInterval first{DateTime::fromYMD(2015, 8, 18),
                       DateTime::fromYMD(2015, 8, 20)};
    TimeInterval second{DateTime::fromYMD(2015, 8, 3),
                        DateTime::fromYMD(2015, 8, 31)};
    CHECK_EQUAL(15, startDateAbsDiff(first, second));
}

TEST(TimeInterval, test_date_diff_should_be_zero_between_same_intervals)
{
    TimeInterval first{DateTime::currentDateTime(),
                       DateTime::currentDateTime()};
    TimeInterval second{DateTime::currentDateTime(),
                        DateTime::currentDateTime()};
    CHECK_EQUAL(0, startDateAbsDiff(first, second));
}
