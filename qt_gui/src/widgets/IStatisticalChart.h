#ifndef I_STATISTAL_CHART_H
#define I_STATISTAL_CHART_H


#include <QWidget>
#include <vector>
#include <string>


/* Widget that is able to illustrate data that is divided in parts proportional
 * to the quantity it represents. */
class IStatisticalChart : public QWidget {
    Q_OBJECT

public:
    using LabelValuePair = std::pair<std::string, double>;

    explicit IStatisticalChart(QWidget* parent = 0)
        : QWidget(parent)
    {
    }

    virtual ~IStatisticalChart();

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


#endif /* end of include guard: I_STATISTAL_CHART_H */
