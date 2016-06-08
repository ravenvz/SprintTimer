#ifndef REQUESTPOMODAILYDISTRIBUTION_H_QUUONEQF
#define REQUESTPOMODAILYDISTRIBUTION_H_QUUONEQF


#include "core/Command.h"
#include "core/IPomodoroDistributionReader.h"

namespace UseCases {

class RequestPomoDistribution : public Command {
public:
    RequestPomoDistribution(IPomodoroDistributionReader& reader,
                            const TimeSpan& timeSpan,
                            IPomodoroDistributionReader::Handler handler)
        : reader{reader}
        , timeSpan{timeSpan}
        , handler{handler}
    {
    }

    void execute() final { reader.requestDailyDistribution(timeSpan, handler); }

    std::string inspect() const final
    {
        return "Request pomodoro distribution";
    }

private:
    IPomodoroDistributionReader& reader;
    const TimeSpan& timeSpan;
    IPomodoroDistributionReader::Handler handler;
};


} /* UseCases */


#endif /* end of include guard: REQUESTPOMODAILYDISTRIBUTION_H_QUUONEQF */
