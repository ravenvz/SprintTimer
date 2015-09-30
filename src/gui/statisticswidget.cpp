#include "statisticswidget.h"
#include "ui_statistics_widget.h"
#include "db_layer/db_helper.h"


StatisticsWidget::StatisticsWidget(Config& applicationSettings, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget),
    applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    currentInterval = ui->widgetPickPeriod->getInterval();
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
    PomodoroStatItem statistics {pomodoros, currentInterval};
    Distribution* weekdayDistribution = statistics.getWeekdayDistribution();
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
    ui->workdayBarChart->setBackground(this->palette().color(QWidget::backgroundRole()));
    ui->workdayBarChart->xAxis->grid()->setVisible(false);
    ui->workdayBarChart->yAxis->grid()->setVisible(false);
    ui->workdayBarChart->xAxis->setAutoSubTicks(false);
    ui->workdayBarChart->xAxis->setSubTickCount(0);
    QPen tickPen;
    tickPen.setStyle(Qt::NoPen);
    ui->workdayBarChart->xAxis->setTickPen(tickPen);
//    ui->workdayBarChart->yAxis->setVisible(false);
}

void StatisticsWidget::updateWeekdayBarChart(Distribution* weekdayDistribution) {
    QVector<double> ticks;
    ticks << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;
    ui->workdayBarChart->xAxis->setTickVector(ticks);
    QVector<QString> labels;
    labels << "" << "Mo" << "Tu" << "We" << "Th" << "Fr" << "Sa" << "Su";
    ui->workdayBarChart->xAxis->setAutoTickLabels(false);
    ui->workdayBarChart->xAxis->setTickVectorLabels(labels);
    ui->workdayBarChart->xAxis->setRange(-0.5, ticks.size() - 1.5);
    ui->workdayBarChart->yAxis->setRange(0, weekdayDistribution->getMax());
    weekdayBarChart->setData(ticks, weekdayDistribution->getDistributionVector());
    ui->workdayBarChart->replot();

    updateWeekdayBarChartLegend(weekdayDistribution);
}

void StatisticsWidget::updateWeekdayBarChartLegend(Distribution* weekdayDistribution) {
    int relativeComparisonInPercent = int(weekdayDistribution->getMax() * 100 / weekdayDistribution->getAverage());
    if (relativeComparisonInPercent == 0) {
        ui->labelBestWorkdayName->setText("No data");
        ui->labelBestWorkdayMsg->setText("");
    } else {
        ui->labelBestWorkdayName->setText(QDate::longDayName(weekdayDistribution->getMaxValueBin() + 1));
        ui->labelBestWorkdayMsg->setText(QString("%1% more than average").arg(relativeComparisonInPercent));
    }
}

void StatisticsWidget::setupTimelineDiagram() {
}

void StatisticsWidget::setupTopTagsDiagram() {
}

void StatisticsWidget::setupWorkHoursDiagram() {
}

