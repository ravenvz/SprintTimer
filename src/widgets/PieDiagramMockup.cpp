#include <iostream>
#include "PieDiagramMockup.h"


PieDiagramMockup::PieDiagramMockup(QWidget* parent)
    : IStatisticalChart(parent)
{
}

void PieDiagramMockup::setData(const std::vector<LabelValuePair>& data)
{
    std::cout << "Setting data" << std::endl;
}

void PieDiagramMockup::togglePartActive(size_t partIndex)
{
    std::cout << "Commanded to change selection to " << partIndex << std::endl;
    // emit partClicked(partIndex);
}

// void PieDiagramMockup::onSelectionChanged(size_t selectionIndex)
// {
//     std::cout << "Selection has been changed" << std::endl;
// }
// {
//     std::cout << "Selection has been changed" << std::endl;
// }
