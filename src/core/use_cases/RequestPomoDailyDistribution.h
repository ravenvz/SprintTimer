#ifndef REQUESTPOMODAILYDISTRIBUTION_H_QUUONEQF
#define REQUESTPOMODAILYDISTRIBUTION_H_QUUONEQF


#include "core/ICommand.h"
#include "core/IPomodoroDailyDistributionReader.h"

namespace UseCases {

class RequestPomoDailyDistribution : public ICommand {
public:
    RequestPomoDailyDistribution(
        IPomodoroDailyDistributionReader& reader,
        const TimeSpan& timeSpan,
        std::function<void(const Distribution<int>&)> handler)
        : reader{reader}
        , timeSpan{timeSpan}
        , handler{handler}
    {
    }

    void execute() final { reader.requestDailyDistribution(timeSpan, handler); }

private:
    IPomodoroDailyDistributionReader& reader;
    const TimeSpan& timeSpan;
    std::function<void(const Distribution<int>&)> handler;
};


} /* UseCases */


#endif /* end of include guard: REQUESTPOMODAILYDISTRIBUTION_H_QUUONEQF */
