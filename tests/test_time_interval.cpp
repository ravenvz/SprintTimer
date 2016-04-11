#include "core/TimeSpan.h"
#include <TestHarness.h>


TEST_GROUP(TimeSpan){

};

TEST(TimeSpan, test_size_in_days_should_be_at_least_one)
{
    DateTime dt = DateTime::fromYMD(2015, 8, 3);
    TimeSpan timeSpan{dt, dt};
    CHECK_EQUAL(1, timeSpan.sizeInDays());
}

TEST(TimeSpan, test_returns_size_in_days)
{
    TimeSpan timeSpan{DateTime::fromYMD(2015, 8, 3),
                          DateTime::fromYMD(2015, 8, 4)};
    CHECK_EQUAL(2, timeSpan.sizeInDays());
}

TEST(TimeSpan, test_returns_size_in_days_from_incorrect_timeSpan)
{
    TimeSpan timeSpan{DateTime::fromYMD(2015, 8, 18),
                          DateTime::fromYMD(2015, 8, 3)};
    CHECK_EQUAL(16, timeSpan.sizeInDays());
}

TEST(TimeSpan, test_returns_correct_date_diff_between_timeSpans_start)
{
    TimeSpan first{DateTime::fromYMD(2015, 8, 3),
                       DateTime::fromYMD(2015, 8, 31)};
    TimeSpan second{DateTime::fromYMD(2015, 8, 18),
                        DateTime::fromYMD(2015, 8, 20)};
    CHECK_EQUAL(15, startDateAbsDiff(first, second));
}

TEST(TimeSpan,
     test_returns_correct_date_diff_between_incorrect_timeSpans_start)
{
    TimeSpan first{DateTime::fromYMD(2015, 8, 18),
                       DateTime::fromYMD(2015, 8, 20)};
    TimeSpan second{DateTime::fromYMD(2015, 8, 3),
                        DateTime::fromYMD(2015, 8, 31)};
    CHECK_EQUAL(15, startDateAbsDiff(first, second));
}

TEST(TimeSpan, test_date_diff_should_be_zero_between_same_timeSpans)
{
    TimeSpan first{DateTime::currentDateTime(),
                       DateTime::currentDateTime()};
    TimeSpan second{DateTime::currentDateTime(),
                        DateTime::currentDateTime()};
    CHECK_EQUAL(0, startDateAbsDiff(first, second));
}
