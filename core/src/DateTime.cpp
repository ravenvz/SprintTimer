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
#include "core/DateTime.h"
#include "core/StringUtils.h"
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>

namespace {
template <typename T>
void pop_back_n(T& container, size_t n)
{
    auto limit = std::min(n, container.size());
    for (size_t i = 0; i < limit; ++i)
        container.pop_back();
}

std::string formatDateTime(const DateTime& dt, std::string&& format);
}


DateTime::DateTime(std::chrono::system_clock::time_point timepoint)
    : time{timepoint}
    , daypoint{date::floor<date::days>(time)}
    , ymd{date::year_month_day(daypoint)}
    , tod{date::make_time(time - daypoint)}
{
}

/* static */
DateTime DateTime::fromYMD(int year, int month, int day)
{
    date::year_month_day dateYMD{date::year(year) / month / day};
    if (!dateYMD.ok())
        throw std::runtime_error("Invalid date");
    std::chrono::system_clock::time_point tp = date::sys_days(dateYMD);
    return DateTime{tp};
}

/* static */
DateTime DateTime::fromTime_t(std::time_t timeT, int offsetFromUtcInSeconds)
{
    return DateTime{std::chrono::system_clock::from_time_t(timeT)
                    + std::chrono::seconds{offsetFromUtcInSeconds}};
}

/* static */
DateTime DateTime::currentDateTime()
{
    return DateTime{std::chrono::system_clock::now()};
}

/* static */
DateTime DateTime::currentDateTimeLocal()
{
    auto timepoint = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(timepoint);
    std::tm localTime = *std::localtime(&t);
    to_time_point(localTime, timepoint);
    return DateTime{timepoint};
}

std::time_t DateTime::toTime_t() const
{
    return std::chrono::system_clock::to_time_t(time);
}

DateTime DateTime::addDays(int days) const
{
    return DateTime{time + date::days{days}};
}

DateTime DateTime::addMonths(int months) const
{
    return DateTime{time + date::months{months}};
}

int DateTime::daysTo(const DateTime& other) const
{
    using namespace date;
    return (floor<days>(other.time) - floor<days>(this->time)).count();
}

std::chrono::system_clock::time_point DateTime::chronoTimepoint() const
{
    return time;
}

int DateTime::year() const { return int(ymd.year()); }

unsigned DateTime::month() const { return unsigned(ymd.month()); }

unsigned DateTime::day() const { return unsigned(ymd.day()); }

long DateTime::hour() const { return tod.hours().count(); }

long DateTime::minute() const { return tod.minutes().count(); }

long long DateTime::second() const { return tod.seconds().count(); }

unsigned DateTime::dayOfWeek() const
{
    std::array<unsigned, 7> mondayFirstTable{{7u, 1u, 2u, 3u, 4u, 5u, 6u}};
    return mondayFirstTable[static_cast<unsigned>(date::weekday(ymd))];
}

std::string DateTime::toString(std::string format) const
{
    return formatDateTime(*this, std::move(format));
}

std::ostream& operator<<(std::ostream& os, const DateTime& dt)
{
    os << std::setfill('0') << std::setw(2) << dt.day() << "."
       << std::setfill('0') << std::setw(2) << dt.month() << "." << dt.year()
       << " " << std::setfill('0') << std::setw(2) << dt.hour() << ":"
       << std::setfill('0') << std::setw(2) << dt.minute() << ":"
       << std::setfill('0') << std::setw(2) << dt.second();
    return os;
}

namespace {

std::string formatDateTime(const DateTime& dt, std::string&& format)
{
    std::stringstream ss;

    std::string f{std::move(format)};
    std::reverse(f.begin(), f.end());

    using namespace StringUtils;

    while (!f.empty()) {
        if (endsWith(f, "''")) {
            ss << "'";
            pop_back_n(f, 2);
        }
        else if (endsWith(f, "'")) {
            pop_back_n(f, 1);
            auto m = f.find_last_of("'");
            if (m != std::string::npos) {
                while (!endsWith(f, "'")) {
                    ss << f.back();
                    f.pop_back();
                }
                f.pop_back();
            }
        }
        else if (endsWith(f, "yyyy")) {
            ss << std::setfill('0') << std::setw(4) << dt.year();
            pop_back_n(f, 4);
        }
        else if (endsWith(f, "yy")) {
            ss << std::setfill('0') << std::setw(2) << dt.year() % 100;
            pop_back_n(f, 2);
        }
        else if (endsWith(f, "MM")) {
            ss << std::setfill('0') << std::setw(2) << dt.month();
            pop_back_n(f, 2);
        }
        else if (endsWith(f, "M")) {
            ss << dt.month();
            pop_back_n(f, 1);
        }
        else if (endsWith(f, "dd")) {
            ss << std::setfill('0') << std::setw(2) << dt.day();
            pop_back_n(f, 2);
        }
        else if (endsWith(f, "d")) {
            ss << dt.day();
            pop_back_n(f, 1);
        }
        else if (endsWith(f, "hh")) {
            ss << std::setfill('0') << std::setw(2) << dt.hour();
            pop_back_n(f, 2);
        }
        else if (endsWith(f, "h")) {
            ss << dt.hour();
            pop_back_n(f, 1);
        }
        else if (endsWith(f, "mm")) {
            ss << std::setfill('0') << std::setw(2) << dt.minute();
            pop_back_n(f, 2);
        }
        else if (endsWith(f, "m")) {
            ss << dt.minute();
            pop_back_n(f, 1);
        }
        else if (endsWith(f, "ss")) {
            ss << std::setfill('0') << std::setw(2) << dt.second();
            pop_back_n(f, 2);
        }
        else if (endsWith(f, "s")) {
            ss << dt.second();
            pop_back_n(f, 1);
        }
        else {
            ss << f.back();
            f.pop_back();
        }
    }

    return ss.str();
}
}
