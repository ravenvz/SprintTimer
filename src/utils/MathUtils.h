#ifndef POMODORO_MATHUTILS_H
#define POMODORO_MATHUTILS_H

#include <QString>
#include <QVector>
#include <QtCore/qvector.h>
#include <QDebug>


class MathUtils {

public:
    static double average(unsigned total, int numBins);
    static QString averageAsQString(unsigned total, int numBins);
    static double percentage(unsigned chunk, unsigned total);
    static QString percentageAsQString(unsigned chunk, unsigned total);
    static unsigned int sum(const QVector<unsigned>& values);

};


class Distribution
{

public:
    Distribution(int numBins) :
        numBins(numBins),
        distribution(QVector<double> (numBins, 0))
    {

    }

    double getAverage() const {
        return average;
    }

    double getMax() const {
        return max;
    }

    int getMaxValueBin() const {
        return maxValueBin;
    }

    double getBinValue(int bin) {
        if (isValidBin(bin))
            return distribution[bin];
        return 0;
    }

    void incrementBinValue(int bin) {
        if (isValidBin(bin)) {
            distribution[bin]++;
        }
    }

    bool isValidBin(int bin) {
        return bin >= 0 && bin < numBins;
    }

    QVector<double> getDistributionVector() {
        return distribution;
    }


protected:
    double average;
    double max = 0;
    int numBins;
    int maxValueBin = 0;
    QVector<double> distribution;

    void computeMaxAndAverage() {
        double total = 0;
        for (int bin = 0; bin < distribution.size(); ++bin) {
            double value = getBinValue(bin);
            total += value;
            if (value > max) {
                max = value;
                maxValueBin = bin;
            }
        }
        average = total / double(numBins);
    }
};

#endif //POMODORO_MATHUTILS_H
