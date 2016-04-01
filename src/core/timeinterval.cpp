#include "timeinterval.h"


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


TimeInterval::TimeInterval(SystemClock start, SystemClock finish)
    : startTime{DateTime{std::move(start)}}
    , finishTime{DateTime{std::move(finish)}}
{
}

TimeInterval::TimeInterval(const DateTime& start, const DateTime& finish)
    : startTime{start}
    , finishTime{finish}
{
}

TimeInterval::TimeInterval(std::time_t start,
                           std::time_t finish,
                           int offsetFromUtcInSeconds)
    : startTime{DateTime::fromTime_t(start, offsetFromUtcInSeconds)}
    , finishTime{DateTime::fromTime_t(finish, offsetFromUtcInSeconds)}
{
}

// TODO remove when got rid of Qt containers
TimeInterval::TimeInterval()
    : startTime{DateTime::currentDateTime()}
    , finishTime{DateTime::currentDateTime()}
{
}

TimeInterval::DayPart TimeInterval::getDayPart() const
{
    auto hour = startTime.hour();

    if (22 < hour || hour <= 2) {
        return DayPart::MIDNIGHT;
    }
    else if (2 < hour && hour <= 6) {
        return DayPart::NIGHT;
    }
    else if (6 < hour && hour <= 10) {
        return DayPart::MORNING;
    }
    else if (10 < hour && hour <= 14) {
        return DayPart::NOON;
    }
    else if (14 < hour && hour <= 18) {
        return DayPart::AFTERNOON;
    }
    else {
        return DayPart::EVENING;
    }
}

/* static */
std::string TimeInterval::dayPartName(int dayPart)
{
    return dayPartName(static_cast<DayPart>(dayPart));
}

/* static */
std::string TimeInterval::dayPartName(DayPart dayPart)
{
    switch (dayPart) {
    case DayPart::MIDNIGHT:
        return "Midnight";
    case DayPart::NIGHT:
        return "Night";
    case DayPart::MORNING:
        return "Morning";
    case DayPart::NOON:
        return "Noon";
    case DayPart::AFTERNOON:
        return "Afternoon";
    case DayPart::EVENING:
        return "Evening";
    }
    return "Invalid";
}

/* static */
std::string TimeInterval::dayPartHours(int dayPart)
{
    return dayPartHours(static_cast<DayPart>(dayPart));
}

/* static */
std::string TimeInterval::dayPartHours(DayPart dayPart)
{
    switch (dayPart) {
    case DayPart::MIDNIGHT:
        return "22:00 - 2:00";
    case DayPart::NIGHT:
        return "2:00 - 6:00";
    case DayPart::MORNING:
        return "6:00 - 10:00";
    case DayPart::NOON:
        return "10:00 - 14:00";
    case DayPart::AFTERNOON:
        return "14:00 - 18:00";
    case DayPart::EVENING:
        return "18:00 - 22:00";
    }
    return "Invalid";
}

std::string TimeInterval::toTimeString() const
{
    std::string res{startTime.toTimeString()};
    res += " - ";
    res += finishTime.toTimeString();
    return res;
}
