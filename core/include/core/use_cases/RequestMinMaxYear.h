/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#ifndef REQUESTMINMAXYEAR_H_0SPNHM7R
#define REQUESTMINMAXYEAR_H_0SPNHM7R

#include "core/Command.h"
#include "core/IYearRangeReader.h"

namespace UseCases {

class RequestMinMaxYear : public Command {
public:
    RequestMinMaxYear(IYearRangeReader& reader,
                      IYearRangeReader::Handler handler)
        : reader{reader}
        , handler{handler}
    {
    }

    void execute() final { reader.requestYearRange(handler); }

    std::string inspect() const final { return "Request min max year"; }

private:
    IYearRangeReader& reader;
    IYearRangeReader::Handler handler;
};


} /* UseCases */

#endif /* end of include guard: REQUESTMINMAXYEAR_H_0SPNHM7R */
