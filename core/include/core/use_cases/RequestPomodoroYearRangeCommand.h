#ifndef REQUESTPOMODOROYEARRANGECOMMAND_H_0SPNHM7R
#define REQUESTPOMODOROYEARRANGECOMMAND_H_0SPNHM7R

#include "core/ICommand.h"
#include "core/IPomodoroYearRangeReader.h"

namespace UseCases {

class RequestPomodoroYearRangeCommand : public ICommand {
public:
    RequestPomodoroYearRangeCommand(IPomodoroYearRangeReader& reader,
                                    IPomodoroYearRangeReader::Handler handler)
        : reader{reader}
        , handler{handler}
    {
    }

    void execute() final { reader.requestYearRange(handler); }

private:
    IPomodoroYearRangeReader& reader;
    IPomodoroYearRangeReader::Handler handler;
};


} /* UseCases */

#endif /* end of include guard: REQUESTPOMODOROYEARRANGECOMMAND_H_0SPNHM7R */
