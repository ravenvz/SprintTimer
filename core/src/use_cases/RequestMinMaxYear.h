#ifndef REQUESTPOMODOROYEARRANGECOMMAND_H_0SPNHM7R
#define REQUESTPOMODOROYEARRANGECOMMAND_H_0SPNHM7R

#include "core/Command.h"
#include "core/IPomodoroYearRangeReader.h"

namespace UseCases {

class RequestMinMaxYear : public Command {
public:
    RequestMinMaxYear(IPomodoroYearRangeReader& reader,
                      IPomodoroYearRangeReader::Handler handler)
        : reader{reader}
        , handler{handler}
    {
    }

    void execute() final { reader.requestYearRange(handler); }

    std::string inspect() const final { return "Request min max year"; }

private:
    IPomodoroYearRangeReader& reader;
    IPomodoroYearRangeReader::Handler handler;
};


} /* UseCases */

#endif /* end of include guard: REQUESTPOMODOROYEARRANGECOMMAND_H_0SPNHM7R */
