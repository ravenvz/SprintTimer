#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include "core/config.h"

namespace Ui {
    class StatisticsWidget;
}

class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(Config& applicationSettings, QWidget* parent = 0);
    ~StatisticsWidget();

private:
    Ui::StatisticsWidget* ui;
    Config& applicationSettings;
    void setupWorkdayBarChart();
};



#endif /* end of include guard: STATISTICSWIDGET_H */
