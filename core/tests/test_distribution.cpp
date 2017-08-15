/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

// TODO remove when Gtest drops std::tr1
// Workaround for C++17 as std::tr1 no longer available and Gtest uses it
#define GTEST_LANG_CXX11 1

#include "core/Distribution.h"
#include "gtest/gtest.h"

TEST(Distribution, test_empty_distribution_initialized_correctly)
{
    unsigned numBins = 7;
    Distribution<double> distribution{numBins};

    EXPECT_DOUBLE_EQ(0, distribution.getAverage());
    EXPECT_DOUBLE_EQ(0, distribution.getMax());
    EXPECT_TRUE(distribution.isValidBin(0));
    EXPECT_TRUE(distribution.isValidBin(6));
    EXPECT_FALSE(distribution.isValidBin(7));
    EXPECT_EQ(0, distribution.getMaxValueBin());

    EXPECT_TRUE(distribution.empty());

    for (unsigned i = 0; i < numBins; ++i) {
        EXPECT_DOUBLE_EQ(0, distribution.getBinValue(i));
    }
}

TEST(Distribution, test_correctly_initializes_from_distribution_vector)
{
    Distribution<double> distribution{
        std::vector<double>{1, 2, 3, 4, 3, 2, 1, 0}};

    EXPECT_EQ(8, distribution.getNumBins());
    EXPECT_DOUBLE_EQ(2, distribution.getAverage());
    EXPECT_DOUBLE_EQ(4, distribution.getMax());
    EXPECT_EQ(3, distribution.getMaxValueBin());
    EXPECT_DOUBLE_EQ(16, distribution.getTotal());
}
