#include "MathUtils.h"

double MathUtils::average(unsigned total, int numBins) {
    return double(total) / numBins;
}

double MathUtils::percentage(unsigned chunk, unsigned total) {
    return total != 0 ? double(chunk) * 100 / total : 0;
}
