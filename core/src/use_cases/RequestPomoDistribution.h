/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
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
