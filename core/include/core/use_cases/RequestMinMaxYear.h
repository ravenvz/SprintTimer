/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#include "core/IYearRangeReader.h"
#include "core/Query.h"

namespace core::use_cases {

class RequestMinMaxYear : public Query {
public:
    RequestMinMaxYear(IYearRangeReader& year_range_reader,
                      IYearRangeReader::Handler handler)
        : reader{year_range_reader}
        , handler_{handler}
    {
    }

    void execute() final { reader.requestYearRange(handler_); }

    std::string describe() const final { return "Request min max year"; }

private:
    IYearRangeReader& reader;
    IYearRangeReader::Handler handler_;
};

} // namespace core::use_cases

#endif /* end of include guard: REQUESTMINMAXYEAR_H_0SPNHM7R */
