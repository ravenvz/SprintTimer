/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include <iostream>
#include <iterator>
#include <vector>

namespace sprint_timer {

template <class T> class Distribution {
public:
    Distribution() = default;

    explicit Distribution(size_t binNumber)
        : distribution{std::vector<T>(binNumber, 0)}
        , binFrequency{std::vector<int>(binNumber, 1)}
    {
    }

    explicit Distribution(std::vector<T>&& distribution_)
        : distribution{std::move(distribution_)}
        , binFrequency{std::vector<int>(distribution.size(), 1)}
    {
        computeMaxAndAverage();
    }

    Distribution(std::vector<T>&& distribution_,
                 std::vector<int>&& binFrequency)
        : distribution{std::move(distribution_)}
        , binFrequency{std::move(binFrequency)}
    {
        normalizeByBinFrequency();
        computeMaxAndAverage();
    }

    size_t getNumBins() const { return distribution.size(); }

    double getAverage() const { return average; }

    double getMax() const { return max; }

    size_t getMaxValueBin() const { return maxValueBin; }

    T getBinValue(size_t bin) const
    {
        if (isValidBin(bin))
            return distribution[bin];
        return 0;
    }

    bool isValidBin(size_t bin) const
    {
        return bin >= 0 && bin < distribution.size();
    }

    std::vector<T> getDistributionVector() const { return distribution; }

    T getTotal() const { return total; }

    bool empty() const { return static_cast<int>(total) == 0; }

private:
    double average{0};
    double max{0};
    size_t maxValueBin{0};
    T total{0};
    std::vector<T> distribution;
    std::vector<int> binFrequency;

    void computeMaxAndAverage()
    {
        if (distribution.empty())
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
        average = sum / double(distribution.size());
    }

    void normalizeByBinFrequency()
    {
        for (size_t bin = 0; bin < distribution.size(); ++bin) {
            if (binFrequency[bin] > 0)
                distribution[bin] /= binFrequency[bin];
        }
    }
};

template <typename T>
bool operator==(const Distribution<T>& lhs, const Distribution<T>& rhs)
{
    return lhs.getDistributionVector() == rhs.getDistributionVector();
}

template <class CharT, class Traits, class T>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const Distribution<T>& distr)
{
    os << "Distribution{"
       << "\n\taverage: " << distr.getAverage() << "\n\tmax: " << distr.getMax()
       << " at bin " << distr.getMaxValueBin()
       << "\n\ttotal: " << distr.getTotal()
       << "\n\tnumbers of bins: " << distr.getNumBins() << "\n\t";
    const auto vec = distr.getDistributionVector();
    std::copy(cbegin(vec), cend(vec), std::ostream_iterator<T>(os, ", "));
    os << "}\n";
    return os;
}

} // namespace sprint_timer

#endif /* end of include guard: DISTRIBUTION_H_HG9WILUN */
