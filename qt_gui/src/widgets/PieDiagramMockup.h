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
#ifndef PIEDIAGRAMMOCKUP_H_CM1PNQL0
#define PIEDIAGRAMMOCKUP_H_CM1PNQL0


#include "IStatisticalChart.h"


class DistributionDiagramMockup : public IStatisticalChart {
    Q_OBJECT
public:
    explicit DistributionDiagramMockup(QWidget* parent);

    void setData(const std::vector<LabelValuePair>& data) override;

    void togglePartActive(size_t selectionIndex) override;

    // protected slots:
    //     void onSelectionChanged(size_t selectionIndex) override;
};


#endif /* end of include guard: PIEDIAGRAMMOCKUP_H_CM1PNQL0 */
