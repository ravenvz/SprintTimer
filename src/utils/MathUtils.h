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

    double getBinValue(int bin) const {
        if (isValidBin(bin))
            return distribution[bin];
        return 0;
    }

    void incrementBinValue(int bin) {
        if (isValidBin(bin)) {
            distribution[bin]++;
        }
    }

    bool isValidBin(int bin) const {
        return bin >= 0 && bin < numBins;
    }

    QVector<T> getDistributionVector() const {
        return distribution;
    }


protected:
    double average = 0;
    double max = 0;
    int numBins = 0;
    int maxValueBin = 0;
    QVector<T> distribution;
    QVector<int> binFrequency;
    QStringList binLabels;

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

    void normalizeByBinFrequency() {
        for (int bin = 0; bin < numBins; ++bin) {
            if (binFrequency[bin] > 0)
                distribution[bin] /= binFrequency[bin];
        }
    }
};

#endif //POMODORO_MATHUTILS_H
