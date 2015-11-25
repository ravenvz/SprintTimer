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
    auto dailyDistribution = statistics.getDailyDistribution();
    auto weekdayDistribution = statistics.getWeekdayDistribution();
    auto workTimeDistribution = statistics.getWorkTimeDistribution();
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
    const double penWidthF = 2.2;
    Graph averageGraph = Graph();
    Graph goalGraph = Graph();
    Graph normalGraph = Graph();

    QPen averagePen;
    averagePen.setColor(Qt::blue);
    averagePen.setStyle(Qt::DotLine);
    averagePen.setWidthF(penWidthF);
    averageGraph.setPen(averagePen);

    QPen normalPen;
    normalPen.setColor(QColor::fromRgb(246, 61, 13, 255));
    normalPen.setWidthF(penWidthF);
    normalGraph.setPen(normalPen);

    QPen goalPen;
    goalPen.setColor(Qt::green);
    goalPen.setStyle(Qt::DashLine);
    goalPen.setWidthF(penWidthF);
    goalGraph.setPen(goalPen);

    ui->dailyTimeline->addGraph(averageGraph);
    ui->dailyTimeline->addGraph(goalGraph);
    ui->dailyTimeline->addGraph(normalGraph);
}

void StatisticsWidget::updateDailyTimelineGraph(Distribution<double>* dailyDistribution) {
    if (dailyDistribution->empty()) {
        ui->dailyTimeline->reset();
    } else {
        double average = dailyDistribution->getAverage();
        double dailyGoal = applicationSettings.getDailyPomodorosGoal();
        auto pomosByDay = dailyDistribution->getDistributionVector();
        GraphData averageData {GraphPoint {0, average},
                               GraphPoint {double(currentInterval.sizeInDays() - 1), average}};
        GraphData goalData {GraphPoint {0, dailyGoal},
                            GraphPoint {double(currentInterval.sizeInDays() - 1), dailyGoal}};
        GraphData normalData;
        for (int i = 0; i < pomosByDay.size(); ++i) {
            normalData.push_back(
                    GraphPoint {double(i),
                                pomosByDay[i],
                                QString("%1").arg(currentInterval.startDate.addDays(i).day())});
        }

        ui->dailyTimeline->setRangeX(0, currentInterval.sizeInDays());
        ui->dailyTimeline->setRangeY(0, dailyDistribution->getMax() + 1);
        ui->dailyTimeline->setGraphData(0, averageData);
        ui->dailyTimeline->setGraphData(1, goalData);
        ui->dailyTimeline->setGraphData(2, normalData);
    }
    ui->dailyTimeline->replot();
    updateDailyTimelineGraphLegend(dailyDistribution);
}

void StatisticsWidget::updateTopTagsDiagram(QVector<Slice>& tagSlices) {
    if (!tagSlices.empty() && tagSlices.last().first == "")
        tagSlices.last().first = "others";
    ui->topTagDiagram->setData(tagSlices);
    ui->topTagDiagram->setLegendTitle("Top tags");
    ui->topTagDiagram->setFont(QFont(".Helvetica Neue Desk UI", 13));
}

void StatisticsWidget::updateDailyTimelineGraphLegend(Distribution<double>* dailyDistribution) {
    ui->labelTotalPomodoros->setText(QString("%1").arg(dailyDistribution->getTotal()));
    ui->labelDailyAverage->setText(QString("%1").arg(dailyDistribution->getAverage(), 2, 'f', 2, '0'));
}

void StatisticsWidget::onSliceSelectionChanged(int newSliceIndex) {
    selectedSliceIndex = selectedSliceIndex == newSliceIndex ? -1 : newSliceIndex;
    drawGraphs();
}
