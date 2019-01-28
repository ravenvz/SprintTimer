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
#include "qt_gui/widgets/PieDiagramMockup.h"
#include <iostream>

// TODO I think it's time for this mockup to go?

namespace sprint_timer::ui::qt_gui {


DistributionDiagramMockup::DistributionDiagramMockup(QWidget* parent)
    : IStatisticalChart(parent)
{
}

void DistributionDiagramMockup::setData(const std::vector<LabelValuePair>& data)
{
    std::cout << "Setting data" << std::endl;
}

void DistributionDiagramMockup::togglePartActive(size_t partIndex)
{
    std::cout << "Commanded to change selection to " << partIndex << std::endl;
    // emit partClicked(partIndex);
}

// void DistributionDiagramMockup::onSelectionChanged(size_t selectionIndex)
// {
//     std::cout << "Selection has been changed" << std::endl;
// }
// {
//     std::cout << "Selection has been changed" << std::endl;
// }

} // namespace sprint_timer::ui::qt_gui

