#include "statisticswidget.h"
#include "ui_statistics_widget.h"
// #include "core/entities/Pomodoro.h"
#include "db_layer/db_helper.h"
//#include "pickperiodwidget.h"


StatisticsWidget::StatisticsWidget(Config& applicationSettings, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget),
    applicationSettings(applicationSettings)
//    weekdayDistribution(pomodoros)
{
    ui->setupUi(this);
    connectSlots();
    setupGraphs();
}

StatisticsWidget::~StatisticsWidget() {
    delete ui;
}

void StatisticsWidget::connectSlots() {
    connect(ui->widgetPickPeriod, SIGNAL(intervalChanged(DateInterval)), this, SLOT(onDatePickerIntervalChanged(DateInterval)));
}

void StatisticsWidget::setupGraphs() {
    setupWeekdayBarChart();
}

void StatisticsWidget::updateGraphs() {
    updateWeekdayBarChart();
}

void StatisticsWidget::onDatePickerIntervalChanged(DateInterval newInterval) {
    currentInterval = newInterval;
    retrievePomodoros();
    computePomodoroWorkdayDistribution();
    updateGraphs();
}

void StatisticsWidget::setupWeekdayBarChart() {
    weekdayBarChart = new QCPBars(ui->workdayBarChart->xAxis, ui->workdayBarChart->yAxis);
    ui->workdayBarChart->addPlottable(weekdayBarChart);

    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(Qt::red);
    weekdayBarChart->setPen(pen);
    weekdayBarChart->setBrush(QBrush(Qt::green));
    ui->workdayBarChart->xAxis->grid()->setVisible(false);
    ui->workdayBarChart->yAxis->grid()->setVisible(false);
    ui->workdayBarChart->xAxis->grid()->setSubGridVisible(false);
    ui->workdayBarChart->xAxis->setVisible(false);
    ui->workdayBarChart->yAxis->setVisible(false);
}

void StatisticsWidget::updateWeekdayBarChart() {
    QVector<double> ticks;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
    ui->workdayBarChart->xAxis->setTickVector(ticks);
//    QVector<QString> labels;
//    labels << "Mo" << "Tu" << "We" << "Th" << "Fr" << "Sa" << "Su";
//    ui->workdayBarChart->xAxis->setAutoTickLabels(false);
//    ui->workdayBarChart->xAxis->setTickVectorLabels(labels);
    ui->workdayBarChart->xAxis->setRange(0, ticks.size() + 1);
    ui->workdayBarChart->yAxis->setRange(0, weekdayDistribution.getMax());
    weekdayBarChart->setData(ticks, weekdayDistribution.getDistributionVector());
    ui->workdayBarChart->replot();

}

void StatisticsWidget::retrievePomodoros() {
    pomodoros = PomodoroDataSource::getPomodorosBetween(currentInterval.startDate, currentInterval.endDate);
}

void StatisticsWidget::computePomodoroWorkdayDistribution() {
    weekdayDistribution = PomoWeekdayDistribution {pomodoros};
}

void StatisticsWidget::setupTimelineDiagram() {
}

void StatisticsWidget::setupTopTagsDiagram() {
}

void StatisticsWidget::setupWorkHoursDiagram() {
}

