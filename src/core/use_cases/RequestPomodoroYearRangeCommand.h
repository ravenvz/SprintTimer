#ifndef REQUESTPOMODOROYEARRANGECOMMAND_H_0SPNHM7R
#define REQUESTPOMODOROYEARRANGECOMMAND_H_0SPNHM7R

#include "core/ICommand.h"
#include "core/IPomodoroYearRangeReader.h"

namespace UseCases {

class RequestPomodoroYearRangeCommand : public ICommand {
public:
    RequestPomodoroYearRangeCommand(IPomodoroYearRangeReader& reader)
        : reader{reader}
    {
    }

    void execute() final { reader.requestYearRange(); }

private:
    IPomodoroYearRangeReader& reader;
};


} /* UseCases */

#endif /* end of include guard: REQUESTPOMODOROYEARRANGECOMMAND_H_0SPNHM7R */
