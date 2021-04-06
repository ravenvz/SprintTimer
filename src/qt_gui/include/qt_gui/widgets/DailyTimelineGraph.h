/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#ifndef DAILYTIMELINEGRAPH_H
#define DAILYTIMELINEGRAPH_H

#include "qt_gui/presentation/DailyStatisticsGraphContract.h"
#include <QFrame>
#include <memory>

namespace Ui {
class DailyTimelineGraph;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class DailyTimelineGraph : public QFrame,
                           public contracts::DailyStatisticGraphContract::View {
public:
    explicit DailyTimelineGraph(QWidget* parent = nullptr);

    ~DailyTimelineGraph() override;

    void clearGraphs() override;

    void drawGraph(const contracts::DailyStatisticGraphContract::GraphData&
                       graphData) override;

private:
    std::unique_ptr<Ui::DailyTimelineGraph> ui;

    void setGraphsVisualOptions(
        const contracts::DailyStatisticGraphContract::GraphData& data);

    void updateLegend(const contracts::DailyStatisticGraphContract::LegendData&
                          legendData) override;
};

} // namespace sprint_timer::ui::qt_gui

#endif // DAILYTIMELINEGRAPH_H
