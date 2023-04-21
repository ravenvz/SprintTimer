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
#include "core/Recurrence.h"
#include "gtest/gtest.h"

using sprint_timer::DomainException;
using sprint_timer::Recurrence;
using sprint_timer::validateRecurrence;
using namespace dw;
using namespace std::chrono_literals;

class RecurrenceFixture : public ::testing::Test {
public:
    DateTime referenceTimeStamp{DateTime{Date{Year{2023}, Month{3}, Day{17}}} +
                                13h + 40min + 28s};
};

// DayOfWeek Year-Month-Day Hour:Minute:Second

TEST_F(RecurrenceFixture, normalizes_pattern)
{
    // TODO add support for lowercase and long day names
    EXPECT_EQ("2024-11-31 23:00:00", Recurrence{"2024-11-31 23:00"}.pattern());
    EXPECT_EQ("*-11-31 23:00:00", Recurrence{"*-11-31 23:00"}.pattern());
    EXPECT_EQ("*-*-* 11:12:00", Recurrence{"11:12"}.pattern());
    EXPECT_EQ("Mon..Thu,Sat,Sun *-*-* 00:00:00",
              Recurrence{"Sat,Thu,Mon..Wed,Sat..Sun"}.pattern());
    EXPECT_EQ("Mon,Sun 2012-*-* 01,02:23:00",
              Recurrence{"Mon,Sun 12-*-* 2,1:23"}.pattern());
    EXPECT_EQ("Wed *-*-01 00:00:00", Recurrence{"Wed *-1"}.pattern());
    EXPECT_EQ("Wed *-*-01 00:00:00", Recurrence{"Wed..Wed,Wed *-1"}.pattern());
    EXPECT_EQ("Wed *-*-* 17:48:00", Recurrence{"Wed, 17:48"}.pattern());
    EXPECT_EQ("Tue..Sat 2012-10-15 01:02:03",
              Recurrence{"Wed..Sat,Tue 12-10-15 1:2:3"}.pattern());
    EXPECT_EQ("*-*-07 00:00:00", Recurrence{"*-*-7 0:0:0"}.pattern());
    EXPECT_EQ("*-10-15 00:00:00", Recurrence{"10-15"}.pattern());
    EXPECT_EQ("Mon *-12-* 17:00:00", Recurrence{"Mon *-12-* 17:00"}.pattern());
    EXPECT_EQ("*-*-* 08..12/4:11:00", Recurrence{"08..14/4:11"}.pattern());
    // EXPECT_EQ("Mon *-12-* 17:00:00",
    //           Recurrence{"monday *-12-* 17:00"}.pattern());
    EXPECT_EQ("Mon,Fri *-*-01,02,03 *:30:45",
              Recurrence{"Mon,Fri *-*-3,1,2 *:30:45"}.pattern());
    EXPECT_EQ("*-*-* 12,13,14:10,20,30:00",
              Recurrence{"12,14,13,12:20,10,30"}.pattern());
    EXPECT_EQ("*-*-* 12..14:10,20,30:00",
              Recurrence{"12..14:10,20,30"}.pattern());
    EXPECT_EQ("Mon,Fri *-01/2-01,03 *:30:45",
              Recurrence{"Mon,Fri *-1/2-1,3 *:30:45"}.pattern());
    // EXPECT_EQ("Mon,Fri *-01/2-01,03 *:30:45",
    //           Recurrence{"mon,fri *-1/2-1,3 *:30:45"}.pattern());
    EXPECT_EQ("*-03-05 08:05:40", Recurrence{"03-05 08:05:40"}.pattern());
    EXPECT_EQ("*-*-* 08:05:40", Recurrence{"08:05:40"}.pattern());
    EXPECT_EQ("*-*-* 05:40:00", Recurrence{"05:40"}.pattern());
    EXPECT_EQ("Sat,Sun *-12-05 08:05:40",
              Recurrence{"Sat,Sun 12-05 08:05:40"}.pattern());
    EXPECT_EQ("Sat,Sun *-*-* 08:05:40",
              Recurrence{"Sat,Sun 08:05:40"}.pattern());
    EXPECT_EQ("2003-03-05 05:40:00", Recurrence{"2003-03-05 05:40"}.pattern());
    // EXPECT_EQ("*-*-* 05:40:23.420000/3.170001",
    //           Recurrence{"05:40:23.4200004/3.1700005"}.pattern());
    EXPECT_EQ("2003-02..04-05 00:00:00",
              Recurrence{"2003-02..04-05"}.pattern());
    // EXPECT_EQ("2003-03-05 05:40:00 UTC",
    //           Recurrence{"2003-03-05 05:40 UTC"}.pattern());
    EXPECT_EQ("2003-03-05 00:00:00", Recurrence{"2003-03-05"}.pattern());
    EXPECT_EQ("*-03-05 00:00:00", Recurrence{"03-05"}.pattern());
    EXPECT_EQ("*-*-* *:00:00", Recurrence{"hourly"}.pattern());
    EXPECT_EQ("*-*-* 00:00:00", Recurrence{"daily"}.pattern());
    // EXPECT_EQ("*-*-* 00:00:00 UTC", Recurrence{"daily UTC"}.pattern());
    EXPECT_EQ("*-*-01 00:00:00", Recurrence{"monthly"}.pattern());
    EXPECT_EQ("Mon *-*-* 00:00:00", Recurrence{"weekly"}.pattern());
    // EXPECT_EQ("Mon *-*-* 00:00:00 Pacific/Auckland",
    //           Recurrence{"weekly Pacific/Auckland"}.pattern());
    EXPECT_EQ("*-01-01 00:00:00", Recurrence{"yearly"}.pattern());
    EXPECT_EQ("*-01-01 00:00:00", Recurrence{"annually"}.pattern());
    EXPECT_EQ("*-*-* *:02/3:00", Recurrence{"*:2/3"}.pattern());
    EXPECT_EQ("2023-01/2-10 11:02:00",
              Recurrence{"2023-1/2-10 11:2"}.pattern());
    EXPECT_EQ("*-01,04,07,10-01 00:00:00", Recurrence{"quarterly"}.pattern());
    EXPECT_EQ("*-01,07-01 00:00:00", Recurrence{"semiannually"}.pattern());
}

TEST_F(RecurrenceFixture, throws_when_day_is_invalid)
{
    ASSERT_THROW(Recurrence{"2023-03-32 12:11:56"}, DomainException);
    ASSERT_THROW(Recurrence{"2023-03-00 12:11:56"}, DomainException);
}

TEST_F(RecurrenceFixture, throws_when_month_is_invalid)
{
    ASSERT_THROW(Recurrence{"2023-13-02 12:11:56"}, DomainException);
    ASSERT_THROW(Recurrence{"2023-00-02 12:11:56"}, DomainException);
}

TEST_F(RecurrenceFixture, throws_when_hour_is_invalid)
{
    ASSERT_THROW(Recurrence{"2023-03-02 24:11:56"}, DomainException);
}

TEST_F(RecurrenceFixture, throws_when_minute_is_invalid)
{
    ASSERT_THROW(Recurrence{"2023-03-02 23:60:56"}, DomainException);
}

TEST_F(RecurrenceFixture, throws_when_second_is_invalid)
{
    ASSERT_THROW(Recurrence{"2023-03-02 23:11:60"}, DomainException);
}

TEST_F(RecurrenceFixture, returns_next_recurrence)
{
    EXPECT_EQ(
        (DateTime{Date{Year{2023}, Month{3}, Day{17}}} + 17h + 55min),
        Recurrence{"2023-03-17 17:55:00"}.nextRecurrence(referenceTimeStamp));
    EXPECT_EQ(
        (DateTime{Date{Year{2023}, Month{3}, Day{17}}} + 13h + 40min + 30s),
        Recurrence{"*-*-* *:*:30"}.nextRecurrence(referenceTimeStamp));
    EXPECT_EQ((DateTime{Date{Year{2023}, Month{4}, Day{7}}} + 17h + 55min),
              Recurrence{"Fri 2023-04-* 17:55:00"}.nextRecurrence(
                  referenceTimeStamp));
    EXPECT_EQ((DateTime{Date{Year{2023}, Month{3}, Day{17}}} + 14h + 00min),
              Recurrence{"*-*-* *:00:00"}.nextRecurrence(referenceTimeStamp));
    EXPECT_EQ(
        (DateTime{Date{Year{2023}, Month{3}, Day{17}}} + 13h + 40min + 28s),
        Recurrence{"*:*:*"}.nextRecurrence(referenceTimeStamp));
    EXPECT_EQ(
        (DateTime{Date{Year{2023}, Month{3}, Day{18}}} + 11h + 12min + 13s),
        Recurrence{"11:12:13"}.nextRecurrence(referenceTimeStamp));
    EXPECT_EQ(
        (DateTime{Date{Year{2023}, Month{3}, Day{17}}} + 13h + 40min + 28s),
        Recurrence{"2023-3-17 13:40:28"}.nextRecurrence(referenceTimeStamp));
    EXPECT_EQ(
        (DateTime{Date{Year{2023}, Month{3}, Day{17}}} + 13h + 40min + 38s),
        Recurrence{"*-*-* *:*:38"}.nextRecurrence(referenceTimeStamp));

    // DateTime referenceTimeStamp{DateTime{Date{Year{2023}, Month{3}, Day{17}}}
    // +
    //                             13h + 40min + 28s};
}

