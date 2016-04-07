#include "core/timeinterval.h"
#include <TestHarness.h>
#include <iostream>


TEST_GROUP(DateTime){

};

TEST(DateTime, test_constructs_correct_object_from_YMD)
{
    DateTime dt = DateTime::fromYMD(2016, 4, 12);
    CHECK_EQUAL(2016, dt.year());
    CHECK_EQUAL(4, dt.month());
    CHECK_EQUAL(12, dt.day());
    CHECK_EQUAL(0, dt.hour());
    CHECK_EQUAL(0, dt.minute());
    CHECK_EQUAL(0, dt.second());
}

TEST(DateTime, test_add_days_forward)
{
    DateTime dt = DateTime::fromYMD(2016, 2, 20).addDays(9);

    CHECK_EQUAL(2016, dt.year());
    CHECK_EQUAL(2, dt.month());
    CHECK_EQUAL(29, dt.day());
}

TEST(DateTime, test_add_days_backward)
{
    DateTime dt = DateTime::fromYMD(2016, 2, 20).addDays(-10);

    CHECK_EQUAL(2016, dt.year());
    CHECK_EQUAL(2, dt.month());
    CHECK_EQUAL(10, dt.day());
}

TEST(DateTime, test_computes_days_forward)
{
    DateTime currentDt = DateTime::currentDateTime();
    CHECK_EQUAL(10, currentDt.daysTo(currentDt.addDays(10)));
}

TEST(DateTime, test_computes_days_backward)
{
    DateTime currentDt = DateTime::currentDateTime();

    CHECK_EQUAL(-11, currentDt.daysTo(currentDt.addDays(-11)));
}


TEST(DateTime, test_returns_correct_weekday_num)
{
    CHECK_EQUAL(1u, DateTime::fromYMD(2016, 4, 4).dayOfWeek());
    CHECK_EQUAL(2u, DateTime::fromYMD(2016, 4, 5).dayOfWeek());
    CHECK_EQUAL(3u, DateTime::fromYMD(2016, 4, 6).dayOfWeek());
    CHECK_EQUAL(4u, DateTime::fromYMD(2016, 4, 7).dayOfWeek());
    CHECK_EQUAL(5u, DateTime::fromYMD(2016, 4, 8).dayOfWeek());
    CHECK_EQUAL(6u, DateTime::fromYMD(2016, 4, 9).dayOfWeek());
    CHECK_EQUAL(7u, DateTime::fromYMD(2016, 4, 10).dayOfWeek());
}
