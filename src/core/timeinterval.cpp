#include "timeinterval.h"


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
