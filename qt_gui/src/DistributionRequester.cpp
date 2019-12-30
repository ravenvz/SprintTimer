/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "qt_gui/DistributionRequester.h"

namespace sprint_timer::ui::qt_gui {

DistributionRequester::DistributionRequester(
    ISprintDistributionReader& reader_,
    QueryInvoker& queryInvoker_,
    const GroupByPeriodStrategy& groupByPeriodStrategy_,
    const BackRequestStrategy& backRequestStrategy_,
    DatasyncRelay& datasyncRelay_,
    QObject* parent)
    : reader{reader_}
    , queryInvoker{queryInvoker_}
    , groupByPeriodStrategy{groupByPeriodStrategy_}
    , backRequestStrategy{backRequestStrategy_}
{
    connect(&datasyncRelay_,
            &DatasyncRelay::dataUpdateRequiered,
            this,
            &DistributionRequester::synchronize);
}

const std::vector<int>& DistributionRequester::distribution() const
{
    return data;
}

void DistributionRequester::synchronize()
{
    using sprint_timer::use_cases::RequestSprintDistribution;
    queryInvoker.execute(std::make_unique<RequestSprintDistribution>(
        reader,
        backRequestStrategy.dateRange(),
        [this](const auto& distribution) {
            data = distribution;
            emit distributionChanged(data);
        }));
}

} // namespace sprint_timer::ui::qt_gui
