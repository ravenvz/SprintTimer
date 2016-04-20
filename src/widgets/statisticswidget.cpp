#include "statisticswidget.h"
#include "barchart.h"
#include "ui_statistics_widget.h"


StatisticsWidget::StatisticsWidget(IConfig& applicationSettings,
                                   QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::StatisticsWidget)
    , applicationSettings(applicationSettings)
{
    setAttribute(Qt::WA_DeleteOnClose);
    pomodoroModel = new PomodoroModel(this);
    ui->setupUi(this);
    ui->widgetPickPeriod->setYears(pomodoroModel->yearRange());
    currentInterval = ui->widgetPickPeriod->getInterval();
    workTimeDiagram = new TimeDiagram(this);
    ui->verticalLayoutBestWorktime->addWidget(workTimeDiagram);
    setupGraphs();
    drawGraphs();
    connectSlots();
}

StatisticsWidget::~StatisticsWidget() { delete ui; }

void StatisticsWidget::connectSlots()
{
    connect(ui->widgetPickPeriod,
            SIGNAL(timeSpanChanged(DateInterval)),
            this,
            SLOT(onDatePickerIntervalChanged(DateInterval)));
    connect(ui->topTagDiagram,
            SIGNAL(chartSelectionChanged(size_t)),
            this,
            SLOT(onTagSelected(size_t)));
}

void StatisticsWidget::updateView()
{
    setupGraphs();
    drawGraphs();
}

void StatisticsWidget::setupGraphs()
{
    fetchPomodoros();
    setupWeekdayBarChart();
    setupDailyTimelineGraph();
}

void StatisticsWidget::fetchPomodoros()
{
    pomodoroModel->setDateFilter(currentInterval);
    pomodoroModel->select();
    // TODO convert in the model
    pomodoros = pomodoroModel->items();
    selectedTagIndex = optional<size_t>();
    tagDistribution = TagDistribution(pomodoros, numTopTags);
    std::vector<TagCount> tagTagCounts = tagDistribution.topTagsDistribution();
    updateTopTagsDiagram(tagTagCounts);
}

void StatisticsWidget::onDatePickerIntervalChanged(DateInterval newInterval)
{
    currentInterval = newInterval;
    fetchPomodoros();
    drawGraphs();
}

void StatisticsWidget::drawGraphs()
{
    PomodoroStatItem statistics{
        selectedTagIndex
            ? tagDistribution.pomodorosForNthTopTag(*selectedTagIndex)
            : pomodoros,
        currentInterval.toTimeSpan()};
    auto dailyDistribution = statistics.dailyDistribution();
    auto weekdayDistribution = statistics.weekdayDistribution();
    auto workTimeDistribution = statistics.worktimeDistribution();
    updateDailyTimelineGraph(dailyDistribution);
    updateWeekdayBarChart(weekdayDistribution);
    updateWorkHoursDiagram(workTimeDistribution, statistics.pomodoros());
}

void StatisticsWidget::setupWeekdayBarChart()
{
    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(Qt::red);
    ui->workdayBarChart->setPen(pen);
    QBrush brush = QBrush(QColor("#73c245"));
    ui->workdayBarChart->setBrush(brush);
}

void StatisticsWidget::updateWeekdayBarChart(
    const Distribution<double>& weekdayDistribution)
{
    std::vector<double> values = weekdayDistribution.getDistributionVector();
    std::vector<QString> labels;
    for (int i = 0; i < 7; ++i) {
        labels.push_back(QDate::shortDayName(i + 1));
    }
    BarData data = BarData(values, labels);
    ui->workdayBarChart->setData(data);
    updateWeekdayBarChartLegend(weekdayDistribution);
}

void StatisticsWidget::updateWeekdayBarChartLegend(
    const Distribution<double>& weekdayDistribution)
{
    if (weekdayDistribution.empty()) {
        ui->labelBestWorkdayName->setText("No data");
        ui->labelBestWorkdayMsg->setText("");
    }
    else {
        double average = weekdayDistribution.getAverage();
        int relativeComparisonInPercent
            = int((weekdayDistribution.getMax() - average) * 100 / average);
        ui->labelBestWorkdayName->setText(QDate::longDayName(
            static_cast<int>(weekdayDistribution.getMaxValueBin()) + 1));
        ui->labelBestWorkdayMsg->setText(
            QString("%1% more than average").arg(relativeComparisonInPercent));
    }
}

void StatisticsWidget::updateWorkHoursDiagram(
    const Distribution<double>& workTimeDistribution,
    const std::vector<Pomodoro>& pomodoros)
{
    std::vector<TimeSpan> timeSpans;
    timeSpans.reserve(pomodoros.size());
    std::transform(pomodoros.cbegin(),
                   pomodoros.cend(),
                   std::back_inserter(timeSpans),
                   [](const auto& pomo) { return pomo.timeSpan(); });
    if (timeSpans.empty()) {
        ui->labelBestWorktimeName->setText("No data");
        ui->labelBestWorktimeHours->setText("");
    }
    else {
        auto maxValueBin
            = static_cast<unsigned>(workTimeDistribution.getMaxValueBin());
        ui->labelBestWorktimeName->setText(
            QString::fromStdString(TimeSpan::dayPartName(maxValueBin)));
        ui->labelBestWorktimeHours->setText(
            QString::fromStdString(TimeSpan::dayPartHours(maxValueBin)));
    }
    workTimeDiagram->setIntervals(timeSpans);
}

void StatisticsWidget::setupDailyTimelineGraph()
{
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
    normalGraph.setShowPoints(true);

    QPen goalPen;
    goalPen.setColor(Qt::green);
    goalPen.setStyle(Qt::DashLine);
    goalPen.setWidthF(penWidthF);
    goalGraph.setPen(goalPen);

    ui->dailyTimeline->addGraph(averageGraph);
    ui->dailyTimeline->addGraph(goalGraph);
    ui->dailyTimeline->addGraph(normalGraph);
}

void StatisticsWidget::updateDailyTimelineGraph(
    const Distribution<double>& dailyDistribution)
{
    if (dailyDistribution.empty()) {
        ui->dailyTimeline->reset();
    }
    else {
        double average = dailyDistribution.getAverage();
        double dailyGoal = applicationSettings.dailyPomodorosGoal();
        auto pomosByDay = dailyDistribution.getDistributionVector();
        GraphData averageData{
            GraphPoint{0, average, ""},
            GraphPoint{static_cast<double>(currentInterval.sizeInDays() - 1),
                       average,
                       ""}};
        GraphData goalData{
            GraphPoint{0, dailyGoal, ""},
            GraphPoint{static_cast<double>(currentInterval.sizeInDays() - 1),
                       dailyGoal,
                       ""}};
        GraphData normalData;
        for (size_t i = 0; i < pomosByDay.size(); ++i) {
            normalData.push_back(
                GraphPoint{double(i),
                           pomosByDay[i],
                           QString("%1").arg(currentInterval.startDate
                                                 .addDays(static_cast<long>(i))
                                                 .day())});
        }

        ui->dailyTimeline->setRangeX(0, currentInterval.sizeInDays());
        ui->dailyTimeline->setRangeY(0, dailyDistribution.getMax() + 1);
        ui->dailyTimeline->setGraphData(0, averageData);
        ui->dailyTimeline->setGraphData(1, goalData);
        ui->dailyTimeline->setGraphData(2, normalData);
    }
    ui->dailyTimeline->replot();
    updateDailyTimelineGraphLegend(dailyDistribution);
}

void StatisticsWidget::updateTopTagsDiagram(std::vector<TagCount>& tagTagCounts)
{
    if (!tagTagCounts.empty() && tagTagCounts.back().first == "")
        tagTagCounts.back().first = "others";
    ui->topTagDiagram->setData(tagTagCounts);
    ui->topTagDiagram->setLegendTitle("Top tags");
    ui->topTagDiagram->setLegendTitleFont(QFont(".Helvetica Neue Desk UI", 13));
}

void StatisticsWidget::updateDailyTimelineGraphLegend(
    const Distribution<double>& dailyDistribution)
{
    ui->labelTotalPomodoros->setText(
        QString("%1").arg(dailyDistribution.getTotal()));
    ui->labelDailyAverage->setText(
        QString("%1").arg(dailyDistribution.getAverage(), 2, 'f', 2, '0'));
}

void StatisticsWidget::onTagSelected(size_t tagIndex)
{
    if (!selectedTagIndex) {
        selectedTagIndex = tagIndex;
    }
    else {
        selectedTagIndex
            = (*selectedTagIndex == tagIndex) ? optional<size_t>() : tagIndex;
    }

    drawGraphs();
}
