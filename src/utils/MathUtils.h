#ifndef POMODORO_MATHUTILS_H
#define POMODORO_MATHUTILS_H

#include <QString>
#include <QVector>


class MathUtils {

public:
    static double average(unsigned total, int numBins);
    static QString averageAsQString(unsigned total, int numBins);
    static double percentage(unsigned chunk, unsigned total);
    static QString percentageAsQString(unsigned chunk, unsigned total);
    static unsigned int sum(const QVector<unsigned>& values);

};


#endif //POMODORO_MATHUTILS_H
