/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "core/TimeSpan.h"
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

TEST(DateTime, test_add_days_backward_2)
{
    DateTime dt = DateTime::fromYMD(2016, 5, 16);
    DateTime thirtyDaysBack = dt.addDays(-30);

    CHECK_EQUAL(2016, thirtyDaysBack.year())
    CHECK_EQUAL(4, thirtyDaysBack.month())
    CHECK_EQUAL(16, thirtyDaysBack.day())
    CHECK_EQUAL(2016, dt.year())
    CHECK_EQUAL(5, dt.month())
    CHECK_EQUAL(16, dt.day())
}

TEST(DateTime, test_add_months_forward)
{
    DateTime dt = DateTime::fromYMD(2016, 5, 17);
    DateTime yearForward = dt.addMonths(12);

    CHECK_EQUAL(2017, yearForward.year());
    CHECK_EQUAL(5, yearForward.month());
    CHECK_EQUAL(17, yearForward.day());
}

TEST(DateTime, test_add_months_backward)
{
    DateTime dt = DateTime::fromYMD(2016, 5, 17);
    DateTime yearBack = dt.addMonths(-12);

    CHECK_EQUAL(2015, yearBack.year());
    CHECK_EQUAL(5, yearBack.month());
    CHECK_EQUAL(17, yearBack.day());
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