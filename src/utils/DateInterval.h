#ifndef DATEINTERVAL_H_DO5P2YKX
#define DATEINTERVAL_H_DO5P2YKX

#include <QDate>

struct DateInterval {
    QDate startDate;
    QDate endDate;

    QString toString() const
    {
        return QString("%1 - %2")
            .arg(startDate.toString())
            .arg(endDate.toString());
    }

    int sizeInDays() const
    {
        return static_cast<int>(startDate.daysTo(endDate) + 1);
    }
};

#endif /* end of include guard: DATEINTERVAL_H_DO5P2YKX */
