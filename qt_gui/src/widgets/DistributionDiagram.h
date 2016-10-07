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
#ifndef DISTRIBUTIONDIAGRAM_H
#define DISTRIBUTIONDIAGRAM_H

#include "widgets/PieChart.h"
#include "widgets/SimpleLegend.h"
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QWidget>
#include <experimental/optional>

using std::experimental::optional;

using DataItem = std::pair<std::string, double>;

class LegendLabel;

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
class DistributionDiagram : public QWidget {
    Q_OBJECT

public:
    explicit DistributionDiagram(QWidget* parent);
    ~DistributionDiagram();

    /* Set data to be displayed. */
    void setData(const std::vector<DataItem>& data);

    /* Set title that is displayed above the legend items. */
    void setLegendTitle(const QString& title);

    /* Set font to use for legend title. */
    void setLegendTitleFont(QFont font);

private slots:
    /* Handle left mouse click on chart part. */
    void onChartPartClicked(size_t partIndex);

    /* Handle left mouse click on legend item. */
    void onLegendItemClicked(size_t itemIndex);

signals:
    /* Emitted when chart part selected/deselected. */
    void chartSelectionChanged(size_t partIndex);

private:
    IStatisticalChart* diagram;
    IStatisticalChartLegend* legend;
    optional<size_t> selectedSliceIndex;
};


#endif // DISTRIBUTIONDIAGRAM_H

