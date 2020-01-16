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
#ifndef DISTRIBUTIONREQUESTER_H_2DNCXVGH
#define DISTRIBUTIONREQUESTER_H_2DNCXVGH

#include "qt_gui/BackRequestStrategy.h"
#include "qt_gui/DatasyncRelay.h"
#include "qt_gui/Synchronizable.h"
#include <QObject>
#include <core/GroupByPeriodStrategy.h>
#include <core/QueryHandler.h>
#include <core/use_cases/request_sprint_distribution/RequestSprintDistributionQuery.h>

namespace sprint_timer::ui::qt_gui {

class DistributionRequester : public QObject, public Synchronizable {

    Q_OBJECT

public:
    DistributionRequester(
        const BackRequestStrategy& backRequestStrategy,
        DatasyncRelay& datasyncRelay,
        QueryHandler<use_cases::RequestSprintDistributionQuery,
                     std::vector<int>>& requestDistributionHandler,
        QObject* parent = nullptr);

    const std::vector<int>& distribution() const;

    void synchronize() override;

signals:
    void distributionChanged(const std::vector<int>&);

private:
    const BackRequestStrategy& backRequestStrategy;
    DatasyncRelay& datasyncRelay;
    QueryHandler<use_cases::RequestSprintDistributionQuery, std::vector<int>>&
        requestDistributionHandler;
    std::vector<int> data;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: DISTRIBUTIONREQUESTER_H_2DNCXVGH */

