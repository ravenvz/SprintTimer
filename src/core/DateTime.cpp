#include "DateTime.h"


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
    std::chrono::system_clock::time_point tp = date::day_point(dateYMD);
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

int DateTime::daysTo(const DateTime& other) const
{
    return (other.ymd.day() - this->ymd.day()).count();
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

long DateTime::second() const { return tod.seconds().count(); }

std::string DateTime::toTimeString() const
{
    using namespace date;
    std::stringstream ss;
    auto tp = floor<std::chrono::minutes>(time);
    auto dp = floor<date::days>(time);
    ss << make_time(tp - dp);
    return ss.str();
}
