#ifndef PIEDIAGRAMMOCKUP_H_CM1PNQL0
#define PIEDIAGRAMMOCKUP_H_CM1PNQL0


#include "IStatisticalChart.h"


class PieDiagramMockup : public IStatisticalChart {
    Q_OBJECT
public:
    explicit PieDiagramMockup(QWidget* parent = 0);

    void setData(const std::vector<LabelValuePair>& data) override;

    void togglePartActive(size_t selectionIndex) override;

    // protected slots:
    //     void onSelectionChanged(size_t selectionIndex) override;
};


#endif /* end of include guard: PIEDIAGRAMMOCKUP_H_CM1PNQL0 */
