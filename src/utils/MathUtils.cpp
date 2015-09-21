#include "MathUtils.h"

double MathUtils::average(unsigned total, int numBins) {
    return double(total) / numBins;
}

unsigned MathUtils::sum(const QVector<unsigned>& values) {
    unsigned int total = 0;
    for (unsigned val : values) {
        total += val;
    }
    return total;
}

QString MathUtils::averageAsQString(unsigned total, int numBins) {
    return QString("%1").arg(MathUtils::average(total, numBins), 2, 'f', 2, '0');
}

double MathUtils::percentage(unsigned chunk, unsigned total) {
    return total != 0 ? double(chunk) * 100 / total : 0;
}

QString MathUtils::percentageAsQString(unsigned chunk, unsigned total) {
    double percentage = MathUtils::percentage(chunk, total);
    return QString("%1%").arg(percentage, 2, 'f', 2, '0');
}

