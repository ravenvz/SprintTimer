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
#ifndef REQUESTSPRINTDISTRIBUTION_H_QUUONEQF
#define REQUESTSPRINTDISTRIBUTION_H_QUUONEQF

#include "core/ISprintDistributionReader.h"
#include "core/Query.h"

namespace sprint_timer::use_cases {

class RequestSprintDistribution : public Query {
public:
    RequestSprintDistribution(ISprintDistributionReader& reader,
                              const dw::DateRange& dateRange,
                              ISprintDistributionReader::Handler handler);

    void execute() final;

    std::string describe() const final;

private:
    ISprintDistributionReader& reader;
    const dw::DateRange dateRange_;
    ISprintDistributionReader::Handler handler_;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: REQUESTSPRINTDISTRIBUTION_H_QUUONEQF */
