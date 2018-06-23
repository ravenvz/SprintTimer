/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#ifndef DISTRIBUTION_H_HG9WILUN
#define DISTRIBUTION_H_HG9WILUN

#include <cstdlib>
#include <vector>

namespace sprint_timer {

template <class T>
class Distribution {
public:
    explicit Distribution(size_t binNumber)
        : numBins{binNumber}
        , distribution{std::vector<T>(binNumber, 0)}
        , binFrequency{std::vector<int>(binNumber, 1)}
    {
    }

    explicit Distribution(std::vector<T>&& distribution_)
        : numBins{distribution_.size()}
        , distribution{std::move(distribution_)}
        , binFrequency{std::vector<int>(distribution.size(), 1)}
    {
        computeMaxAndAverage();
    }

    Distribution(std::vector<T>&& distribution_, std::vector<int>&& binFrequency)
        : numBins{distribution_.size()}
        , distribution{std::move(distribution_)}
        , binFrequency{std::move(binFrequency)}
    {
        normalizeByBinFrequency();
        computeMaxAndAverage();
    }

    size_t getNumBins() const { return numBins; }

    double getAverage() const { return average; }

    double getMax() const { return max; }

    size_t getMaxValueBin() const { return maxValueBin; }

    T getBinValue(size_t bin) const
    {
        if (isValidBin(bin))
            return distribution[bin];
        return 0;
    }

    bool isValidBin(size_t bin) const { return bin >= 0 && bin < numBins; }

    std::vector<T> getDistributionVector() const { return distribution; }

    T getTotal() const { return total; }

    bool empty() const { return static_cast<int>(total) == 0; }

private:
    double average{0};
    double max{0};
    const size_t numBins{0};
    size_t maxValueBin{0};
    T total{0};
    std::vector<T> distribution;
    std::vector<int> binFrequency;

    void computeMaxAndAverage()
    {
        if (numBins == 0)
            return;
        // TODO what if there is an overflow?
        T sum = 0;
        for (size_t bin = 0; bin < distribution.size(); ++bin) {
            auto value = getBinValue(bin);
            sum += value;
            if (value > max) {
                max = value;
                maxValueBin = bin;
            }
        }
        total = sum;
        average = sum / double(numBins);
    }

    void normalizeByBinFrequency()
    {
        for (size_t bin = 0; bin < numBins; ++bin) {
            if (binFrequency[bin] > 0)
                distribution[bin] /= binFrequency[bin];
        }
    }
};

} // namespace sprint_timer

#endif /* end of include guard: DISTRIBUTION_H_HG9WILUN */
