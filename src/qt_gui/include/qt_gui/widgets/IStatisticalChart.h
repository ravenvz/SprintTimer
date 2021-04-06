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
#ifndef I_STATISTAL_CHART_H
#define I_STATISTAL_CHART_H

#include <QWidget>
#include <vector>
#include <string>

namespace sprint_timer::ui::qt_gui {

/* Widget that is able to illustrate data that is divided in parts proportional
 * to the quantity it represents. */
class IStatisticalChart : public QWidget {
    Q_OBJECT

public:
    using LabelValuePair = std::pair<std::string, double>;

    explicit IStatisticalChart(QWidget* parent)
        : QWidget(parent)
    {
    }

    ~IStatisticalChart() override;

    /* Set data that is to be displayed as a chart.
     * Sum of all values is treated as 100%. */
    virtual void setData(const std::vector<LabelValuePair>& data) = 0;

    /* Toggle a part of chart (i.e. slice for pie chart) as active/inactive,
     * so it can be dealt with (i.e. displayed) in a special way. */
    virtual void togglePartActive(size_t) = 0;

    // protected slots:
    //     virtual void onSelectionChanged(size_t selectionIndex) = 0;

signals:
    /* Emitted when part of the chart is clicked with left mouse button. */
    void partClicked(size_t);
};

} // namespace sprint_timer::ui::qt_gui



#endif /* end of include guard: I_STATISTAL_CHART_H */
