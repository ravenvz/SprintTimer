/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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

TEST(DateTime, test_throws_runtime_error_if_invalid_date)
{
    CHECK_THROWS(std::runtime_error, DateTime::fromYMD(2016, 77, 12));
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

TEST(DateTime, test_add_years_forward)
{
    DateTime dt = DateTime::fromYMD(2016, 5, 17);
    DateTime yearForward = dt.addYears(14);

    CHECK_EQUAL(2030, yearForward.year());
    CHECK_EQUAL(5, yearForward.month());
    CHECK_EQUAL(17, yearForward.day());
}

TEST(DateTime, test_add_years_backward)
{
    DateTime dt = DateTime::fromYMD(2016, 5, 17);
    DateTime yearBackward = dt.addYears(-16);

    CHECK_EQUAL(2000, yearBackward.year());
    CHECK_EQUAL(5, yearBackward.month());
    CHECK_EQUAL(17, yearBackward.day());
}

TEST(DateTime, test_add_minutes_forward)
{
    DateTime dt = DateTime::fromYMD(2016, 11, 26);
    DateTime minutesForward = dt.addMinutes(25);

    CHECK_EQUAL(2016, minutesForward.year());
    CHECK_EQUAL(11, minutesForward.month());
    CHECK_EQUAL(26, minutesForward.day());
    CHECK_EQUAL(25, minutesForward.minute());
}

TEST(DateTime, test_add_minutes_backward)
{
    DateTime dt = DateTime::fromYMD(2016, 11, 26);
    DateTime minutesBackward = dt.addMinutes(-25);

    CHECK_EQUAL(2016, minutesBackward.year());
    CHECK_EQUAL(11, minutesBackward.month());
    CHECK_EQUAL(25, minutesBackward.day());
    CHECK_EQUAL(35, minutesBackward.minute());
}

TEST(DateTime, test_add_hours_forward)
{
    DateTime dt = DateTime::fromYMD(2016, 11, 26);
    DateTime hoursForward = dt.addHours(25);

    CHECK_EQUAL(2016, hoursForward.year());
    CHECK_EQUAL(11, hoursForward.month());
    CHECK_EQUAL(27, hoursForward.day());
    CHECK_EQUAL(1, hoursForward.hour());
}

TEST(DateTime, test_add_hours_backward)
{
    DateTime dt = DateTime::fromYMD(2016, 11, 26);
    DateTime hoursBackward = dt.addHours(-25);

    CHECK_EQUAL(2016, hoursBackward.year());
    CHECK_EQUAL(11, hoursBackward.month());
    CHECK_EQUAL(24, hoursBackward.day());
    CHECK_EQUAL(23, hoursBackward.hour());
}

TEST(DateTime, test_add_months_backward)
{
    DateTime dt = DateTime::fromYMD(2016, 5, 17);
    DateTime yearBack = dt.addMonths(-12);

    CHECK_EQUAL(2015, yearBack.year());
    CHECK_EQUAL(5, yearBack.month());
    CHECK_EQUAL(17, yearBack.day());
}

TEST(DateTime, test_computes_distance_forward)
{
    DateTime dt = DateTime::fromYMD(2016, 11, 26);

    CHECK_EQUAL(0, dt.secondsTo(dt.addSeconds(0)));
    CHECK_EQUAL(75, dt.secondsTo(dt.addSeconds(75)));
    CHECK_EQUAL(-75, dt.secondsTo(dt.addSeconds(-75)));

    CHECK_EQUAL(0, dt.minutesTo(dt.addMinutes(0)));
    CHECK_EQUAL(65, dt.minutesTo(dt.addMinutes(65)));
    CHECK_EQUAL(-65, dt.minutesTo(dt.addMinutes(-65)));

    CHECK_EQUAL(0, dt.hoursTo(dt.addHours(0)));
    CHECK_EQUAL(1000, dt.hoursTo(dt.addHours(1000)));
    CHECK_EQUAL(-1000, dt.hoursTo(dt.addHours(-1000)));

    CHECK_EQUAL(0, dt.daysTo(dt));
    CHECK_EQUAL(10, dt.daysTo(dt.addDays(10)));
    CHECK_EQUAL(-10, dt.daysTo(dt.addDays(-10)));

    CHECK_EQUAL(0, dt.monthsTo(dt.addMonths(0)));
    CHECK_EQUAL(12, dt.monthsTo(dt.addMonths(12)));
    CHECK_EQUAL(-12, dt.monthsTo(dt.addMonths(-12)));

    CHECK_EQUAL(0, dt.yearsTo(dt.addYears(0)));
    CHECK_EQUAL(2, dt.yearsTo(dt.addYears(2)));
    CHECK_EQUAL(-2, dt.yearsTo(dt.addYears(-2)));
}

TEST(DateTime, test_computes_days_backward)
{
    DateTime currentDt = DateTime::currentDateTime();

    CHECK_EQUAL(-11, currentDt.daysTo(currentDt.addDays(-11)));
}

TEST(DateTime, test_returns_correct_weekday)
{
    CHECK(DateTime::Weekday::Monday
          == DateTime::fromYMD(2016, 4, 4).dayOfWeek());
    CHECK(DateTime::Weekday::Tuesday
          == DateTime::fromYMD(2016, 4, 5).dayOfWeek());
    CHECK(DateTime::Weekday::Wednesday
          == DateTime::fromYMD(2016, 4, 6).dayOfWeek());
    CHECK(DateTime::Weekday::Thursday
          == DateTime::fromYMD(2016, 4, 7).dayOfWeek());
    CHECK(DateTime::Weekday::Friday
          == DateTime::fromYMD(2016, 4, 8).dayOfWeek());
    CHECK(DateTime::Weekday::Saturday
          == DateTime::fromYMD(2016, 4, 9).dayOfWeek());
    CHECK(DateTime::Weekday::Sunday
          == DateTime::fromYMD(2016, 4, 10).dayOfWeek());
}

TEST(DateTime, test_ostream_operator)
{
    std::tm tm_struct;
    tm_struct.tm_year = 116;
    tm_struct.tm_mon = 8;
    tm_struct.tm_mday = 21;
    tm_struct.tm_hour = 12;
    tm_struct.tm_min = 59;
    tm_struct.tm_sec = 19;
    std::chrono::system_clock::time_point timepoint;
    to_time_point(tm_struct, timepoint);
    DateTime dt{timepoint};
    std::string expected{"21.09.2016 12:59:19"};
    std::stringstream os;

    os << dt;

    CHECK_EQUAL(expected, os.str());
}

TEST(DateTime, test_to_string_handles_date_format)
{
    DateTime dt = DateTime::fromYMD(2016, 9, 21);

    CHECK_EQUAL("2016.09.21", dt.toString("yyyy.MM.dd"));
    CHECK_EQUAL("201616", dt.toString("yyyyyy"));
    CHECK_EQUAL("21", dt.toString("d"));
    CHECK_EQUAL("2121", dt.toString("ddd"));
    CHECK_EQUAL("9", dt.toString("M"));
    CHECK_EQUAL("099", dt.toString("MMM"));
}

TEST(DateTime, test_to_string_handles_time_format)
{
    std::tm tm_struct;
    tm_struct.tm_year = 116;
    tm_struct.tm_mon = 8;
    tm_struct.tm_mday = 21;
    tm_struct.tm_hour = 9;
    tm_struct.tm_min = 7;
    tm_struct.tm_sec = 5;
    std::chrono::system_clock::time_point timepoint;
    to_time_point(tm_struct, timepoint);
    DateTime dt{timepoint};

    CHECK_EQUAL("09", dt.toString("hh"));
    CHECK_EQUAL("9", dt.toString("h"));
    CHECK_EQUAL("07", dt.toString("mm"));
    CHECK_EQUAL("077", dt.toString("mmm"));
    CHECK_EQUAL("077", dt.toString("mmm"));
    CHECK_EQUAL("05", dt.toString("ss"));
    CHECK_EQUAL("055", dt.toString("sss"));
    CHECK_EQUAL("0907", dt.toString("hhmm"));
}

TEST(DateTime, test_ignores_single_quote_that_has_no_pair)
{
    DateTime dt = DateTime::fromYMD(2016, 9, 21);

    CHECK_EQUAL("21-09-2016", dt.toString("dd-'MM-yyyy"))
}

TEST(DateTime, test_threats_input_in_single_quotes_as_text)
{
    DateTime dt = DateTime::fromYMD(2016, 9, 21);

    CHECK_EQUAL("2016|what|09ahhMM21", dt.toString("yyyy|'what'|MM'ahhMM'dd"));
}

TEST(DateTime, test_double_single_quotes_replaced_by_single_quote_in_output)
{
    DateTime dt = DateTime::fromYMD(2016, 9, 21);

    CHECK_EQUAL("2016'09'21", dt.toString("yyyy''MM''dd"));
}

TEST(DateTime, test_comparison_operators)
{
    DateTime dt = DateTime::fromYMD(2016, 11, 29);
    DateTime before = DateTime::fromYMD(2016, 11, 28);
    DateTime after = DateTime::fromYMD(2016, 11, 30);

    CHECK(dt == dt);
    CHECK(dt <= dt);
    CHECK(dt >= dt);
    CHECK(before <= dt);
    CHECK(before < dt);
    CHECK(after >= dt);
    CHECK(after > dt);
}
