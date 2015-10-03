#ifndef POMODORO_MATHUTILS_H
#define POMODORO_MATHUTILS_H

#include <QString>
#include <QVector>
#include <QtCore/qvector.h>
#include <QDebug>


class MathUtils {

public:
    static double percentage(unsigned chunk, unsigned total);
};


template <class T>
class Distribution
{

public:
    explicit Distribution(int binNumber) :
        numBins(binNumber),
        distribution(QVector<T> (binNumber, 0)),
        binFrequency(QVector<int> (binNumber, 1))
    {

    }

    explicit Distribution(QVector<T> distribution) :
        numBins(distribution.size()),
        distribution(distribution),
        binFrequency(QVector<int> (distribution.size(), 1))
    {
        computeMaxAndAverage();
    }

    Distribution(QVector<T> distribution, QVector<int> binFrequency) :
        numBins(distribution.size()),
        distribution(distribution),
        binFrequency(binFrequency)
    {
        normalizeByBinFrequency();
        computeMaxAndAverage();
    }

    int getNumBins() const {
        return numBins;
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

    T getBinValue(int bin) const {
        if (isValidBin(bin))
            return distribution[bin];
        return 0;
    }

    bool isValidBin(int bin) const {
        return bin >= 0 && bin < numBins;
    }

    QVector<T> getDistributionVector() const {
        return distribution;
    }

    T getTotal() const {
        return total;
    }


private:
    double average = 0;
    double max = 0;
    int numBins = 0;
    int maxValueBin = 0;
    T total = 0;
    QVector<T> distribution;
    QVector<int> binFrequency;
    QStringList binLabels;

    void computeMaxAndAverage() {
        if (numBins == 0) return;
        // TODO what if there is an overflow?
        T sum = 0;
        for (int bin = 0; bin < distribution.size(); ++bin) {
            double value = getBinValue(bin);
            sum += value;
            if (value > max) {
                max = value;
                maxValueBin = bin;
            }
        }
        total = sum;
        average = sum / double(numBins);
    }

    void normalizeByBinFrequency() {
        for (int bin = 0; bin < numBins; ++bin) {
            if (binFrequency[bin] > 0)
                distribution[bin] /= binFrequency[bin];
        }
    }
};

#endif //POMODORO_MATHUTILS_H
