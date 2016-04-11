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

unsigned TimeInterval::sizeInDays() const
{
    if (startTime <= finishTime) {
        return static_cast<unsigned>(startTime.daysTo(finishTime) + 1);
    }
    else {
        return static_cast<unsigned>(finishTime.daysTo(startTime) + 1);
    }
}

TimeInterval::DayPart TimeInterval::getDayPart() const
{
    auto hour = startTime.hour();

    if (22 < hour || hour <= 2) {
        return DayPart::Midnight;
    }
    else if (2 < hour && hour <= 6) {
        return DayPart::Night;
    }
    else if (6 < hour && hour <= 10) {
        return DayPart::Morning;
    }
    else if (10 < hour && hour <= 14) {
        return DayPart::Noon;
    }
    else if (14 < hour && hour <= 18) {
        return DayPart::Afternoon;
    }
    else {
        return DayPart::Evening;
    }
}

/* static */
std::string TimeInterval::dayPartName(unsigned dayPart)
{
    return dayPartName(static_cast<DayPart>(dayPart));
}

/* static */
std::string TimeInterval::dayPartName(DayPart dayPart)
{
    switch (dayPart) {
    case DayPart::Midnight:
        return "Midnight";
    case DayPart::Night:
        return "Night";
    case DayPart::Morning:
        return "Morning";
    case DayPart::Noon:
        return "Noon";
    case DayPart::Afternoon:
        return "Afternoon";
    case DayPart::Evening:
        return "Evening";
    }
    return "Invalid";
}

/* static */
std::string TimeInterval::dayPartHours(unsigned dayPart)
{
    return dayPartHours(static_cast<DayPart>(dayPart));
}

/* static */
std::string TimeInterval::dayPartHours(DayPart dayPart)
{
    switch (dayPart) {
    case DayPart::Midnight:
        return "22:00 - 2:00";
    case DayPart::Night:
        return "2:00 - 6:00";
    case DayPart::Morning:
        return "6:00 - 10:00";
    case DayPart::Noon:
        return "10:00 - 14:00";
    case DayPart::Afternoon:
        return "14:00 - 18:00";
    case DayPart::Evening:
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

unsigned startDateAbsDiff(const TimeInterval& one, const TimeInterval& other)
{
    if (one.startTime <= other.startTime) {
        return static_cast<unsigned>(one.startTime.daysTo(other.startTime));
    }
    else {
        return static_cast<unsigned>(other.startTime.daysTo(one.startTime));
    }
}
