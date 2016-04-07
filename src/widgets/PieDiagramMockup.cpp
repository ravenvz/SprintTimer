#include <iostream>
#include "PieDiagramMockup.h"


TopTagDiagramMockup::TopTagDiagramMockup(QWidget* parent)
    : IStatisticalChart(parent)
{
}

void TopTagDiagramMockup::setData(const std::vector<LabelValuePair>& data)
{
    std::cout << "Setting data" << std::endl;
}

void TopTagDiagramMockup::togglePartActive(size_t partIndex)
{
    std::cout << "Commanded to change selection to " << partIndex << std::endl;
    // emit partClicked(partIndex);
}

// void TopTagDiagramMockup::onSelectionChanged(size_t selectionIndex)
// {
//     std::cout << "Selection has been changed" << std::endl;
// }
// {
//     std::cout << "Selection has been changed" << std::endl;
// }
