#ifndef DISTRIBUTION_H_HG9WILUN
#define DISTRIBUTION_H_HG9WILUN

#include <vector>
#include <cstdlib>


template <class T>
class Distribution {

public:
    explicit Distribution(size_t binNumber)
        : numBins(binNumber)
        , distribution(std::vector<T>(binNumber, 0))
        , binFrequency(std::vector<int>(binNumber, 1))
    {
    }

    explicit Distribution(std::vector<T>&& distribution)
        : numBins(distribution.size())
        , distribution(std::move(distribution))
        , binFrequency(std::vector<int>(distribution.size(), 1))
    {
        computeMaxAndAverage();
    }

    Distribution(std::vector<T>&& distribution, std::vector<int>&& binFrequency)
        : numBins(distribution.size())
        , distribution(std::move(distribution))
        , binFrequency(std::move(binFrequency))
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
    size_t numBins{0};
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

    void normalizeByBinFrequency()
    {
        for (size_t bin = 0; bin < numBins; ++bin) {
            if (binFrequency[bin] > 0)
                distribution[bin] /= binFrequency[bin];
        }
    }
};

#endif /* end of include guard: DISTRIBUTION_H_HG9WILUN */
