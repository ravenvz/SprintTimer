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
    const BackRequestStrategy& backRequestStrategy_,
    DatasyncRelay& datasyncRelay_,
    QueryHandler<use_cases::RequestSprintDistributionQuery, std::vector<int>>&
        requestDistributionHandler_,
    QObject* parent)
    : backRequestStrategy{backRequestStrategy_}
    , datasyncRelay{datasyncRelay_}
    , requestDistributionHandler{requestDistributionHandler_}
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
    const auto distribution = requestDistributionHandler.handle(
        use_cases::RequestSprintDistributionQuery{
            backRequestStrategy.dateRange()});
    datasyncRelay.onSyncCompleted("DistributionRequester");
    data = std::move(distribution);
    emit distributionChanged(data);
}

} // namespace sprint_timer::ui::qt_gui
