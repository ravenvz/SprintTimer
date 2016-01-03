#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include <QDateTime>

struct TimeInterval {
    QDateTime startTime;
    QDateTime finishTime;

    enum class DayPart {
        MIDNIGHT,
        NIGHT,
        MORNING,
        NOON,
        AFTERNOON,
        EVENING
    };

    DayPart getDayPart() const {
        int hour = startTime.time().hour();
        if (22 < hour || hour <= 2) {
            return DayPart::MIDNIGHT;
        } else if (2 < hour && hour <= 6) {
            return DayPart::NIGHT;
        } else if (6 < hour && hour <= 10) {
            return DayPart::MORNING;
        } else if (10 < hour && hour <= 14) {
            return DayPart::NOON;
        } else if (14 < hour && hour <= 18) {
            return DayPart::AFTERNOON;
        } else {
            return DayPart::EVENING;
        }
    }

    static QString dayPartName(int dayPart) {
        return dayPartName(static_cast<DayPart>(dayPart));
    }

    static QString dayPartName(DayPart dayPart) {
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
            default:
                return "Invalid";
        }
    }

    static QString dayPartHours(int dayPart) {
        return dayPartHours(static_cast<DayPart>(dayPart));
    }

    static QString dayPartHours(DayPart dayPart) {
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
            default:
                return "Invalid";
        }
    }

    QString toString() const {
        return QString("%1 - %2").arg(startTime.toString()).arg(finishTime.toString());
    }

    QString toTimeString() const {
        return QString("%1 - %2").arg(startTime.toString("HH:mm")).arg(finishTime.toString("HH:mm"));
    }
};

#endif /* end of include guard: TIMEINTERVAL_H */
