#include <iostream>
#include "PieDiagramMockup.h"


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
