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
#ifndef PROGRESSMODEL_H_TFV3K2PH
#define PROGRESSMODEL_H_TFV3K2PH

#include "qt_gui/Synchronizable.h"
#include "qt_gui/WorkdaysChangeListener.h"
#include "qt_gui/models/WorkdayTrackerModel.h"
#include <QObject>
#include <core/ISprintDistributionReader.h>
#include <core/ProgressOverPeriod.h>
#include <core/QueryInvoker.h>
#include <core/use_cases/RequestSprintDistribution.h>

namespace sprint_timer::ui::qt_gui {

class ProgressModel : public QObject,
                      public Synchronizable,
                      public WorkdaysChangeListener {

    Q_OBJECT

public:
    ProgressModel(const GroupingStrategy& groupingStrategy,
                  QueryInvoker& queryInvoker,
                  ISprintDistributionReader& distributionReader,
                  const WorkdayTrackerModel& workdaysModel,
                  QObject* parent = nullptr)
        : groupingStrategy{groupingStrategy}
        , queryInvoker{queryInvoker}
        , reader{distributionReader}
        , workdaysModel{workdaysModel}
    {
        synchronize();
    }

    void synchronize() override
    {
        using sprint_timer::use_cases::RequestSprintDistribution;
        queryInvoker.execute(std::make_unique<RequestSprintDistribution>(
            reader,
            groupingStrategy.dateRange(),
            [this](const Distribution<int>& updatedDistribution) {
                onDataReceived(updatedDistribution);
            }));
    }

    void onWorkdayTrackerChanged(const WorkdayTracker& newTracker) override
    {
        progressData = std::make_unique<ProgressOverPeriod>(
            distribution, workdaysModel.workdayTracker(), groupingStrategy);
    }

private:
    const GroupingStrategy& groupingStrategy;
    QueryInvoker& queryInvoker;
    ISprintDistributionReader& reader;
    const WorkdayTrackerModel& workdaysModel;
    std::unique_ptr<ProgressOverPeriod> progressData;
    Distribution<int> distribution;

    void onDataReceived(const Distribution<int>& updatedDistribution)
    {
        distribution = updatedDistribution;
        progressData = std::make_unique<ProgressOverPeriod>(
            distribution, workdaysModel.workdayTracker(), groupingStrategy);
    }
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PROGRESSMODEL_H_TFV3K2PH */
