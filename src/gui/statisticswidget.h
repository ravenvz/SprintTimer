#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QVector>
#include <external_libs/qcustomplot-source/qcustomplot.h>
#include "gui/dialogs/datepickdialog.h"
#include "core/config.h"
#include <src/core/PomodoroStatistics.h>

namespace Ui {
    class StatisticsWidget;
}

class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(Config& applicationSettings, QWidget* parent = 0);
    ~StatisticsWidget();

private slots:
    void onDatePickerIntervalChanged(DateInterval newInterval);

private:
    Ui::StatisticsWidget* ui;
    Config& applicationSettings;
    DateInterval currentInterval;
    QPointer<QCPBars> weekdayBarChart;

    void connectSlots();
    void setupGraphs();
    void drawGraphs();
    void setupWeekdayBarChart();
    void setupTimelineDiagram();
    void setupTopTagsDiagram();
    void setupWorkHoursDiagram();
    void updateWeekdayBarChart(PomoWeekdayDistribution& weekdayDistribution);
    void updateWeekdayBarChartLegend(PomoWeekdayDistribution& weekdayDistribution) const;
};



#endif /* end of include guard: STATISTICSWIDGET_H */
