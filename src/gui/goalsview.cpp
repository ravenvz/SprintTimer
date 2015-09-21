#include "goalsview.h"
#include "ui_goalsview.h"
#include "gauge.h"
#include "db_layer/db_helper.h"
#include "utils/MathUtils.h"


GoalsView::GoalsView(Config& applicationSettings, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::GoalsView),
    applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    // TODO handle the case when these vectors are empty (e.g. db returns no values)
    lastThirty = PomodoroGateway::getNumCompletedPomodorosForLastThirtyDays();
    lastQuarter = PomodoroGateway::getCompletedPomodorosDistributionForLastThreeMonths();
    lastYear = PomodoroGateway::getCompletedPomodorosDistributionForLastTwelveMonths();
    lastThirtyTotal = MathUtils::sum(lastThirty);
    lastQuarterTotal = MathUtils::sum(lastQuarter);
    lastYearTotal = MathUtils::sum(lastYear);
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
    ui->labelLastMonthAverage->setText(MathUtils::averageAsQString(lastThirtyTotal, lastThirty.size()));
    ui->labelLastQuarterAverage->setText(MathUtils::averageAsQString(lastQuarterTotal, lastQuarter.size()));
    ui->labelLastYearAverage->setText(MathUtils::averageAsQString(lastYearTotal, lastYear.size()));
    ui->labelLastMonthPercentage->setText(MathUtils::percentageAsQString(lastThirtyTotal, monthlyGoal));
    ui->labelLastQuarterPercentage->setText(MathUtils::percentageAsQString(lastQuarterTotal, lastQuarter.size() * weeklyGoal));
    ui->labelLastYearPercentage->setText(MathUtils::percentageAsQString(lastYearTotal, lastYear.size() * monthlyGoal));
    ui->labelTodayProgress->setText(QString("%1").arg(lastThirtyTotal));
    ui->labelWeekProgress->setText(QString("%1").arg(lastQuarterTotal));
    ui->labelMonthProgress->setText(QString("%1").arg(lastYearTotal));
    updateProgressBar(ui->progressBarToday, dailyGoal, lastThirty.last());
    updateProgressBar(ui->progressBarWeek, weeklyGoal, lastQuarter.last());
    updateProgressBar(ui->progressBarMonth, monthlyGoal, lastYear.last());
    drawDiagrams();
}

void GoalsView::updateProgressBar(QProgressBar* bar, unsigned goal, int value) {
    bar->setMaximum(goal);
    bar->setFormat("%v/%m");
    bar->hide();
    if (goal == 0) {
        return;
    }
    QPalette p = bar->palette();
    if (value == (int) goal) {
        p.setColor(QPalette::Highlight, targetGoalReached);
        p.setColor(QPalette::Base, targetGoalReached);
    } else if (value > (int) goal) {
        p.setColor(QPalette::Highlight, overwork);
        p.setColor(QPalette::Base, targetGoalReached);
        bar->setFormat(QString("%1").arg(value) + QString("/%m"));
        value %= goal;
    } else {
        p.setColor(QPalette::Highlight, workInProgress);
        p.setColor(QPalette::Base, Qt::white);
    }
    bar->setPalette(p);
    bar->setValue(value);
    bar->show();
}

void GoalsView::drawDiagrams() {
    drawLastMonthDiagram();
    drawLastQuarterDiagram();
    drawLastYearDiagram();
}

void GoalsView::drawLastMonthDiagram() {
    clearDiagramLayout(ui->gridLayoutLastMonthDiagram);
    unsigned dailyGoal = applicationSettings.getDailyPomodorosGoal();
    for (int row = 0, ind = 0; row < 3; ++row) {
        for (int col = 0; col < 10; ++col, ++ind) {
            ui->gridLayoutLastMonthDiagram->addWidget(new Gauge(lastThirty.at(ind), dailyGoal, this), row, col);
        }
    }
}

void GoalsView::drawLastQuarterDiagram() {
    clearDiagramLayout(ui->gridLayoutLastQuarterDiagram);
    unsigned weeklyGoal = applicationSettings.getWeeklyPomodorosGoal();
    for (int row = 0, ind = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col, ++ind) {
            ui->gridLayoutLastQuarterDiagram->addWidget(new Gauge(lastQuarter.at(ind), weeklyGoal, this), row, col);
        }
    }
}

void GoalsView::drawLastYearDiagram() {
    clearDiagramLayout(ui->gridLayoutLastYearDiagram);
    unsigned monthlyGoal = applicationSettings.getMonthlyPomodorosGoal();
    for (int row = 0, ind = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col, ++ind) {
            ui->gridLayoutLastYearDiagram->addWidget(new Gauge(lastYear.at(ind), monthlyGoal, this), row, col);
        }
    }
}

void GoalsView::clearDiagramLayout(QGridLayout* layout) {
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
}

void GoalsView::updateDailyGoal() {
    applicationSettings.setDailyPomodorosGoal(ui->spinBoxDailyGoal->value());
    displayData();
    drawLastMonthDiagram();
}

void GoalsView::updateWeeklyGoal() {
    applicationSettings.setWeeklyPomodorosGoal(ui->spinBoxWeeklyGoal->value());
    displayData();
    drawLastQuarterDiagram();
}

void GoalsView::updateMonthlyGoal() {
    applicationSettings.setMonthlyPomodorosGoal(ui->spinBoxMonthlyGoal->value());
    displayData();
    drawLastYearDiagram();
}
