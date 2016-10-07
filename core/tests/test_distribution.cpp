/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "core/Distribution.h"
#include <TestHarness.h>

TEST_GROUP(Distribution) { const double tolerance = 0.00001; };

TEST(Distribution, test_empty_distribution_initialized_correctly)
{
    unsigned numBins = 7;
    Distribution<double> distribution{numBins};

    DOUBLES_EQUAL(0u, distribution.getAverage(), tolerance)
    DOUBLES_EQUAL(0u, distribution.getMax(), tolerance)
    CHECK_TRUE(distribution.isValidBin(0u))
    CHECK_TRUE(distribution.isValidBin(6u))
    CHECK_FALSE(distribution.isValidBin(7u))
    CHECK_EQUAL(0u, distribution.getMaxValueBin())

    CHECK(distribution.empty())

    for (unsigned i = 0; i < numBins; ++i) {
        DOUBLES_EQUAL(0, distribution.getBinValue(i), tolerance)
    }
}

TEST(Distribution, test_correctly_initializes_from_distribution_vector)
{
    Distribution<double> distribution{
        std::vector<double>{1, 2, 3, 4, 3, 2, 1, 0}};

    CHECK_EQUAL(8, distribution.getNumBins())
    DOUBLES_EQUAL(2, distribution.getAverage(), tolerance)
    DOUBLES_EQUAL(4, distribution.getMax(), tolerance)
    CHECK_EQUAL(3, distribution.getMaxValueBin())
    DOUBLES_EQUAL(16, distribution.getTotal(), tolerance)
}
