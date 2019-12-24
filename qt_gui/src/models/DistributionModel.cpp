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
#include "qt_gui/models/DistributionModel.h"

namespace sprint_timer::ui::qt_gui {

DistributionModel::DistributionModel(
    ISprintDistributionReader& reader_,
    QueryInvoker& queryInvoker_,
    const ProgressGroupingStrategy& distributionRequestStrategy_,
    const ProgressRangeRequestStrategy& requestStrategy_,
    DatasyncRelay& datasyncRelay_,
    QObject* parent)
    : reader{reader_}
    , queryInvoker{queryInvoker_}
    , distributionRequestStrategy{distributionRequestStrategy_}
    , requestStrategy{requestStrategy_}
{
    connect(&datasyncRelay_,
            &DatasyncRelay::dataUpdateRequiered,
            this,
            &DistributionModel::synchronize);
}

const std::vector<int>& DistributionModel::distribution() const { return data; }

void DistributionModel::synchronize()
{
    using sprint_timer::use_cases::RequestSprintDistribution;
    queryInvoker.execute(std::make_unique<RequestSprintDistribution>(
        reader, requestStrategy.dateRange(), [this](const auto& distribution) {
            data = distribution;
            emit distributionChanged(data);
        }));
}

} // namespace sprint_timer::ui::qt_gui
