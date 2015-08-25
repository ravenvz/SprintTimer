#include "goalsview.h"
#include "ui_goalsview.h"
#include "gauge.h"
#include "db_layer/db_helper.h"
#include <QDebug>


GoalsView::GoalsView(Config& applicationSettings, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::GoalsView),
    applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    lastThirty = PomodoroGateway::getNumCompletedPomodorosForLastThirtyDays();
    lastQuarter = PomodoroGateway::getCompletedPomodorosDistributionForLastThreeMonths();
    lastYear = PomodoroGateway::getCompletedPomodorosDistributionForLastTwelveMonths();
    lastThirtyTotal = computeTotal(lastThirty);
    lastQuarterTotal = computeTotal(lastQuarter);
    lastYearTotal = computeTotal(lastYear);
    displayData();
    connectSlots();
}

GoalsView::~GoalsView() {
    delete ui;
}

void GoalsView::connectSlots() {
    connect(ui->spinBoxDailyGoal, SIGNAL(valueChanged(int)), this, SLOT(updateDailyGoal()));
    connect(ui->spinBoxWeeklyGoal, SIGNAL(valueChanged(int)), this, SLOT(updateWeeklyGoal()));
    connect(ui->spinBoxMonthlyGoal, SIGNAL(valueChanged(int)), this, SLOT(updateMonthlyGoal()));
}

void GoalsView::displayData() {
    unsigned dailyGoal = applicationSettings.getDailyPomodorosGoal();
    unsigned weeklyGoal = applicationSettings.getWeeklyPomodorosGoal();
    unsigned monthlyGoal = applicationSettings.getMonthlyPomodorosGoal();
    ui->spinBoxDailyGoal->setValue(dailyGoal);
    ui->spinBoxWeeklyGoal->setValue(weeklyGoal);
    ui->spinBoxMonthlyGoal->setValue(monthlyGoal);
    ui->labelLastMonthAverage->setText(computeAverage(lastThirtyTotal, lastThirty.size()));
    ui->labelLastQuarterAverage->setText(computeAverage(lastQuarterTotal, lastQuarter.size()));
    ui->labelLastYearAverage->setText(computeAverage(lastYearTotal, lastYear.size()));
    ui->labelLastMonthPercentage->setText(computePercentage(lastThirtyTotal, lastThirty.size(), dailyGoal));
    ui->labelLastQuarterPercentage->setText(computePercentage(lastQuarterTotal, lastQuarter.size(), weeklyGoal));
    ui->labelLastYearPercentage->setText(computePercentage(lastYearTotal, lastYear.size(), monthlyGoal));
    drawDiagrams();
}

unsigned GoalsView::computeTotal(QVector<unsigned>& pomodoroDistribution) {
    unsigned int total = 0;
    for (unsigned numCompleted : pomodoroDistribution) {
        total += numCompleted;
    }
    return total;
}

QString GoalsView::computeAverage(unsigned total, unsigned size) {
    return QString("%1").arg(double(total) / size, 2, 'f', 2, '0');
}

QString GoalsView::computePercentage(unsigned total, unsigned size, unsigned goal) {
    return QString("%1%").arg(double(total) * 100 / (goal * size), 2, 'f', 2, '0');
}

void GoalsView::drawDiagrams() {
    unsigned dailyGoal = applicationSettings.getDailyPomodorosGoal();
    unsigned weeklyGoal = applicationSettings.getWeeklyPomodorosGoal();
    unsigned monthlyGoal = applicationSettings.getMonthlyPomodorosGoal();
    for (int row = 0, ind = 0; row < 3; ++row) {
        for (int col = 0; col < 10; ++col, ++ind) {
            ui->gridLayoutLastMonthDiagram->addWidget(new Gauge(lastThirty.at(ind), dailyGoal, this), row, col);
        }
    }
    for (int row = 0, ind = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col, ++ind) {
            ui->gridLayoutLastQuarterDiagram->addWidget(new Gauge(lastQuarter.at(ind), weeklyGoal, this), row, col);
            ui->gridLayoutLastYearDiagram->addWidget(new Gauge(lastYear.at(ind), monthlyGoal, this), row, col);
        }
    }
}

void GoalsView::updateDailyGoal() {
    applicationSettings.setDailyPomodorosGoal(ui->spinBoxDailyGoal->value());
    displayData();
    drawDiagrams();
}

void GoalsView::updateWeeklyGoal() {
    applicationSettings.setWeeklyPomodorosGoal(ui->spinBoxWeeklyGoal->value());
    displayData();
    drawDiagrams();
}

void GoalsView::updateMonthlyGoal() {
    applicationSettings.setMonthlyPomodorosGoal(ui->spinBoxMonthlyGoal->value());
    displayData();
    drawDiagrams();
}
