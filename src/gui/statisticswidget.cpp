#include "statisticswidget.h"
#include "ui_statistics_widget.h"
#include "gui/barchart.h"


StatisticsWidget::StatisticsWidget(Config& applicationSettings, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWidget),
    applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    currentInterval = ui->widgetPickPeriod->getInterval();
    workTimeDiagram = new TimeDiagram(this);
    ui->verticalLayoutBestWorktime->addWidget(workTimeDiagram);
    setupGraphs();
    drawGraphs();
    connectSlots();
}

StatisticsWidget::~StatisticsWidget() {
    delete ui;
}

void StatisticsWidget::connectSlots() {
    connect(ui->widgetPickPeriod, SIGNAL(intervalChanged(DateInterval)), this, SLOT(onDatePickerIntervalChanged(DateInterval)));
    connect(ui->topTagDiagram, SIGNAL(sliceSelectionChanged(int)), this, SLOT(onSliceSelectionChanged(int)));
}

void StatisticsWidget::setupGraphs() {
    fetchPomodoros();
    setupWeekdayBarChart();
    setupDailyTimelineGraph();
}

void StatisticsWidget::fetchPomodoros() {
    pomodoros = PomodoroDataSource::getPomodorosBetween(currentInterval.startDate,
                                                        currentInterval.endDate);
    selectedSliceIndex = -1;
    tagPomoMap = TagPomoMap(pomodoros, numDisplayedTagSlices);
    QVector<Slice> tagSlices = tagPomoMap.getSortedSliceVector();
    updateTopTagsDiagram(tagSlices);
}

void StatisticsWidget::onDatePickerIntervalChanged(DateInterval newInterval) {
    currentInterval = newInterval;
    fetchPomodoros();
    drawGraphs();
}

void StatisticsWidget::drawGraphs() {
    PomodoroStatItem statistics {
        selectedSliceIndex == -1 ? pomodoros : tagPomoMap.getPomodorosForSlice(selectedSliceIndex),
        currentInterval};
    Distribution<double>* dailyDistribution = statistics.getDailyDistribution();
    Distribution<double>* weekdayDistribution = statistics.getWeekdayDistribution();
    Distribution<double>* workTimeDistribution = statistics.getWorkTimeDistribution();
    updateDailyTimelineGraph(dailyDistribution);
    updateWeekdayBarChart(weekdayDistribution);
    updateWorkHoursDiagram(workTimeDistribution, statistics.getPomodoros());
}

void StatisticsWidget::setupWeekdayBarChart() {
    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(Qt::red);
    ui->workdayBarChart->setPen(pen);
    QBrush brush = QBrush(QColor("#73c245"));
    ui->workdayBarChart->setBrush(brush);
}

void StatisticsWidget::updateWeekdayBarChart(Distribution<double>* weekdayDistribution) {
    QVector<double> values = weekdayDistribution->getDistributionVector();
    QVector<QString> labels;
    for (int i = 0; i < 7; ++i) {
        labels.push_back(QDate::shortDayName(i + 1));
    }
    BarData data = BarData(values, labels);
    ui->workdayBarChart->setData(data);
    updateWeekdayBarChartLegend(weekdayDistribution);
}

void StatisticsWidget::updateWeekdayBarChartLegend(Distribution<double>* weekdayDistribution) {
    if (weekdayDistribution->empty()) {
        ui->labelBestWorkdayName->setText("No data");
        ui->labelBestWorkdayMsg->setText("");
    } else {
        double average = weekdayDistribution->getAverage();
        int relativeComparisonInPercent = int((weekdayDistribution->getMax() - average) * 100 / average);
        ui->labelBestWorkdayName->setText(QDate::longDayName(weekdayDistribution->getMaxValueBin() + 1));
        ui->labelBestWorkdayMsg->setText(QString("%1% more than average").arg(relativeComparisonInPercent));
    }
}

void StatisticsWidget::updateWorkHoursDiagram(Distribution<double>* workTimeDistribution, const QVector<Pomodoro>& pomodoros) {
    QVector<TimeInterval> intervals;
    for (const Pomodoro& pomo : pomodoros) {
        intervals.append(TimeInterval {pomo.getStartTime(), pomo.getFinishTime()});
    }
    if (intervals.empty()) {
        ui->labelBestWorktimeName->setText("No data");
        ui->labelBestWorktimeHours->setText("");
    } else {
        ui->labelBestWorktimeName->setText(
                QString("%1").arg(TimeInterval::getDayPartName(workTimeDistribution->getMaxValueBin())));
        ui->labelBestWorktimeHours->setText(
                QString("%1").arg(TimeInterval::getDayPartHours(workTimeDistribution->getMaxValueBin())));
    }
    workTimeDiagram->setIntervals(intervals);
}

void StatisticsWidget::setupDailyTimelineGraph() {
//    dailyTimeline = new QCPGraph(ui->dailyTimelineGraph->xAxis, ui->dailyTimelineGraph->yAxis);
    ui->dailyTimelineGraph->addGraph();
    ui->dailyTimelineGraph->addGraph();
    ui->dailyTimelineGraph->addGraph();
    ui->dailyTimelineGraph->xAxis->grid()->setVisible(false);
    ui->dailyTimelineGraph->yAxis->grid()->setVisible(false);
    QPen normalPen;
    normalPen.setColor(QColor::fromRgb(246, 61, 13, 255));
    normalPen.setWidthF(2.2);
    ui->dailyTimelineGraph->graph(0)->setPen(normalPen);
    QPen averagePen;
    averagePen.setStyle(Qt::DotLine);
    ui->dailyTimelineGraph->graph(1)->setPen(averagePen);
    QPen goalPen;
    goalPen.setColor(Qt::green);
    goalPen.setStyle(Qt::DashLine);
    ui->dailyTimelineGraph->graph(2)->setPen(goalPen);
    ui->dailyTimelineGraph->setBackground(this->palette().color(QWidget::backgroundRole()));
//    ui->dailyTimelineGraph->addPlottable(dailyTimeline);
}

void StatisticsWidget::updateTopTagsDiagram(QVector<Slice>& tagSlices) {
    if (!tagSlices.empty() && tagSlices.last().first == "")
        tagSlices.last().first = "others";
    ui->topTagDiagram->setData(tagSlices);
    ui->topTagDiagram->setLegendTitle("Top tags");
    ui->topTagDiagram->setFont(QFont(".Helvetica Neue Desk UI", 13));
}

void StatisticsWidget::updateDailyTimelineGraph(Distribution<double>* dailyDistribution) {
    QVector<double> ticks;
    for (int i = 0; i < dailyDistribution->getNumBins(); ++i) {
        ticks << i;
    }
    QVector<double> completedPomodoros = dailyDistribution->getDistributionVector();
    QVector<double> average (dailyDistribution->getNumBins(), dailyDistribution->getAverage());
    QVector<double> dailyGoal (dailyDistribution->getNumBins(), applicationSettings.getDailyPomodorosGoal());
    ui->dailyTimelineGraph->graph(0)->setData(ticks, completedPomodoros);
    ui->dailyTimelineGraph->graph(1)->setData(ticks, average);
    ui->dailyTimelineGraph->graph(2)->setData(ticks, dailyGoal);
    ui->dailyTimelineGraph->xAxis->setRange(0, currentInterval.sizeInDays() - 1);
    ui->dailyTimelineGraph->yAxis->setRange(0, dailyDistribution->getMax() + 3);
    ui->dailyTimelineGraph->replot();

    updateDailyTimelineGraphLegend(dailyDistribution);
}

void StatisticsWidget::updateDailyTimelineGraphLegend(Distribution<double>* dailyDistribution) {
    ui->labelTotalPomodoros->setText(QString("%1").arg(dailyDistribution->getTotal()));
    ui->labelDailyAverage->setText(QString("%1").arg(dailyDistribution->getAverage(), 2, 'f', 2, '0'));
}

void StatisticsWidget::onSliceSelectionChanged(int newSliceIndex) {
    selectedSliceIndex = selectedSliceIndex == newSliceIndex ? -1 : newSliceIndex;
    drawGraphs();
}
