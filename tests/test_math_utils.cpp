#include "utils/MathUtils.h"
#include <TestHarness.h>

TEST_GROUP(Distribution) {
    const double tolerance = 0.00001;
};

TEST(Distribution, test_empty_distribution_initialized_correctly) {
    int numBins = 7;
    Distribution<double> distribution {numBins};

    DOUBLES_EQUAL(0, distribution.getAverage(), tolerance)
    DOUBLES_EQUAL(0, distribution.getMax(), tolerance)
    CHECK_TRUE(distribution.isValidBin(0))
    CHECK_TRUE(distribution.isValidBin(6))
    CHECK_FALSE(distribution.isValidBin(-1))
    CHECK_FALSE(distribution.isValidBin(7))
    CHECK_EQUAL(0, distribution.getMaxValueBin())

    for (int i = 0; i < numBins; ++i) {
        DOUBLES_EQUAL(0, distribution.getBinValue(i), tolerance)
    }
}

TEST(Distribution, test_correctly_initializes_from_distribution_vector) {
    QVector<double> distributionVector {1, 2, 3, 4, 3, 2, 1, 0};
    Distribution<double> distribution {distributionVector};

    CHECK_EQUAL(8, distribution.getNumBins())
    DOUBLES_EQUAL(2, distribution.getAverage(), tolerance)
    DOUBLES_EQUAL(4, distribution.getMax(), tolerance)
    CHECK_EQUAL(3, distribution.getMaxValueBin())
}