#ifndef POMODORO_TAGDIAGRAM_H
#define POMODORO_TAGDIAGRAM_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>
#include "widgets/PieChart.h"
#include "widgets/SimpleLegend.h"
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
    explicit DistributionDiagram(QWidget* parent = 0);
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


#endif // POMODORO_TAGDIAGRAM_H
