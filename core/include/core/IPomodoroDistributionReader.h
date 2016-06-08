#ifndef IPOMODORODAILYDISTRIBUTIONREADER_H_XMW7JEI6
#define IPOMODORODAILYDISTRIBUTIONREADER_H_XMW7JEI6

#include "core/Distribution.h"
#include "core/TimeSpan.h"
#include <functional>

class IPomodoroDistributionReader {
public:
    using Handler = std::function<void(const Distribution<int>&)>;
    virtual ~IPomodoroDistributionReader() = default;
    // TODO rename to requestDistribution
    virtual void requestDailyDistribution(const TimeSpan& timeSpan,
                                          Handler handler)
        = 0;
};


#endif /* end of include guard: IPOMODORODAILYDISTRIBUTIONREADER_H_XMW7JEI6 */
