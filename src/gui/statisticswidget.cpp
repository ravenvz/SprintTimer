#include "statisticswidget.h"
#include "ui_statistics_widget.h"
#include "db_layer/db_helper.h"


StatisticsWidget::StatisticsWidget(Config& applicationSettings, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget),
    applicationSettings(applicationSettings),
    currentInterval(DateInterval {QDate::currentDate().addDays(-30), QDate::currentDate()})
{
    ui->setupUi(this);
    ui->widgetPickPeriod->setInterval(currentInterval);
    setupGraphs();
    drawGraphs();
    connectSlots();
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

void StatisticsWidget::onDatePickerIntervalChanged(DateInterval newInterval) {
    currentInterval = newInterval;
    drawGraphs();
}

void StatisticsWidget::drawGraphs() {
    QVector<Pomodoro> pomodoros = PomodoroDataSource::getPomodorosBetween(currentInterval.startDate,
                                                                          currentInterval.endDate);
    PomoWeekdayDistribution weekdayDistribution = PomoWeekdayDistribution {pomodoros};
    updateWeekdayBarChart(weekdayDistribution);
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

void StatisticsWidget::updateWeekdayBarChart(PomoWeekdayDistribution& weekdayDistribution) {
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

    updateWeekdayBarChartLegend(weekdayDistribution);
}

void StatisticsWidget::updateWeekdayBarChartLegend(PomoWeekdayDistribution& weekdayDistribution) const {
    ui->labelBestWorkdayName->setText(QDate::longDayName(weekdayDistribution.getMaxValueBin() + 1));
    ui->labelBestWorkdayMsg->setText(QString("%1% more than average")
         .arg(int(weekdayDistribution.getMax() * 100 / weekdayDistribution.getAverage())));
}

void StatisticsWidget::setupTimelineDiagram() {
}

void StatisticsWidget::setupTopTagsDiagram() {
}

void StatisticsWidget::setupWorkHoursDiagram() {
}

