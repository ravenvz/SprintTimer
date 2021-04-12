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
#ifndef TOPTAGDIAGRAM_H_IABMCEKV
#define TOPTAGDIAGRAM_H_IABMCEKV

#include "qt_gui/presentation/TopTagDiagramContract.h"
#include <QMouseEvent>
#include <QWidget>

namespace sprint_timer::ui::qt_gui {

using DataItem = std::pair<std::string, double>;

class LegendLabel;

class PieChart;

class IStatisticalChartLegend;

/* Displays information about data distribution relative to some quantity.
 *
 * It consists of graphical chart and legend user can interact with.
 * It can use any graphical chart that implements IStatisticalChart
 * interface.
 *
 * When legend item or graphical chart (depends on implementation)
 * part is clicked or selected, chartSelectionChanged(size_t partIndex) signal
 * is emitted, indicating index of chart part being clicked.
 */
class TopTagDiagram : public QWidget,
                      public contracts::TopTagDiagramContract::View {
public:
    explicit TopTagDiagram(QWidget* parent = nullptr);

    ~TopTagDiagram() override;

    /* Set title that is displayed above the legend items. */
    void setLegendTitle(const QString& title);

    /* Set font to use for legend title. */
    void setLegendTitleFont(QFont font);

    void updateLegend(const std::vector<std::string>& tagNames) override;

    void updateDiagram(
        std::vector<contracts::TopTagDiagramContract::DiagramData>&& data)
        override;

    void toggleSelection(std::optional<size_t> selection) override;

private:
    PieChart* diagram;
    IStatisticalChartLegend* legend;

    /* Handle left mouse click on chart part. */
    void onChartPartClicked(size_t partIndex);

    /* Handle left mouse click on legend item. */
    void onLegendItemClicked(size_t itemIndex);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TOPTAGDIAGRAM_H_IABMCEKV */
