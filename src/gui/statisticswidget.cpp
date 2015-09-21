#include <external_libs/qcustomplot-source/qcustomplot.h>
#include "statisticswidget.h"
#include "ui_statistics_widget.h"


StatisticsWidget::StatisticsWidget(Config& applicationSettings, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget),
    applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    setupWorkdayBarChart();
}

StatisticsWidget::~StatisticsWidget() {
    delete ui;
}

void StatisticsWidget::setupWorkdayBarChart() {
    QCPBars* bar = new QCPBars(ui->workdayBarChart->xAxis, ui->workdayBarChart->yAxis);
    ui->workdayBarChart->addPlottable(bar);

    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(Qt::red);
    bar->setPen(pen);
    bar->setBrush(QBrush(Qt::green));
    QVector<double> ticks;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
    ui->workdayBarChart->xAxis->setTickVector(ticks);
    ui->workdayBarChart->xAxis->setRange(0, 10);

    QVector<double> distribution;
    distribution << 4 << 8 << 11 << 22 << 9 << 0 << 4;

    bar->setData(ticks, distribution);
}
