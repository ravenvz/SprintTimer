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
    Distribution<unsigned>* lastDays = goalStatistics.getDistributionForLastThirtyDays();
    Distribution<unsigned>* lastWeeks = goalStatistics.getDistributionForLastTwelveWeeks();
    Distribution<unsigned>* lastMonths = goalStatistics.getDistributionForLastTwelveMonths();
    ui->spinBoxDailyGoal->setValue(dailyGoal);
    ui->spinBoxWeeklyGoal->setValue(weeklyGoal);
    ui->spinBoxMonthlyGoal->setValue(monthlyGoal);
    ui->labelLastMonthAverage->setText(formatDecimal(lastDays->getAverage()));
    ui->labelLastQuarterAverage->setText(formatDecimal(lastWeeks->getAverage()));
    ui->labelLastYearAverage->setText(formatDecimal(lastMonths->getAverage()));
    ui->labelLastMonthPercentage->setText(QString("%1%")
            .arg(formatDecimal(MathUtils::percentage(lastDays->getTotal(), monthlyGoal))));
    ui->labelLastQuarterPercentage->setText(QString("%1%")
            .arg(formatDecimal(MathUtils::percentage(lastWeeks->getTotal(), lastWeeks->getNumBins() * weeklyGoal))));
    ui->labelLastYearPercentage->setText(QString("%1%")
            .arg(formatDecimal(MathUtils::percentage(lastMonths->getTotal(), lastMonths->getNumBins() * monthlyGoal))));
    ui->labelTodayProgress->setText(QString("%1").arg(lastDays->getTotal()));
    ui->labelWeekProgress->setText(QString("%1").arg(lastWeeks->getTotal()));
    ui->labelMonthProgress->setText(QString("%1").arg(lastMonths->getTotal()));
    updateProgressBar(ui->progressBarToday, dailyGoal, lastDays->getBinValue(lastDays->getNumBins() - 1));
    updateProgressBar(ui->progressBarWeek, weeklyGoal, lastWeeks->getBinValue(lastWeeks->getNumBins() - 1));
    updateProgressBar(ui->progressBarMonth, monthlyGoal, lastMonths->getBinValue(
            lastMonths->getNumBins() - 1));
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
    Distribution<unsigned>* distribution = goalStatistics.getDistributionForLastThirtyDays();
    unsigned dailyGoal = applicationSettings.getDailyPomodorosGoal();
    for (int row = 0, ind = 0; row < 3; ++row) {
        for (int col = 0; col < 10; ++col, ++ind) {
            ui->gridLayoutLastMonthDiagram->addWidget(new Gauge(distribution->getBinValue(ind), dailyGoal, this), row, col);
        }
    }
}

void GoalsView::drawLastQuarterDiagram() {
    clearDiagramLayout(ui->gridLayoutLastQuarterDiagram);
    Distribution<unsigned>* distribution = goalStatistics.getDistributionForLastTwelveWeeks();
    unsigned weeklyGoal = applicationSettings.getWeeklyPomodorosGoal();
    for (int row = 0, ind = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col, ++ind) {
            ui->gridLayoutLastQuarterDiagram->addWidget(new Gauge(distribution->getBinValue(ind), weeklyGoal, this), row, col);
        }
    }
}

void GoalsView::drawLastYearDiagram() {
    clearDiagramLayout(ui->gridLayoutLastYearDiagram);
    Distribution<unsigned>* distribution = goalStatistics.getDistributionForLastTwelveMonths();
    unsigned monthlyGoal = applicationSettings.getMonthlyPomodorosGoal();
    for (int row = 0, ind = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col, ++ind) {
            ui->gridLayoutLastYearDiagram->addWidget(new Gauge(distribution->getBinValue(ind), monthlyGoal, this), row, col);
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
    applicationSettings.setDailyPomodorosGoal(unsigned(ui->spinBoxDailyGoal->value()));
    displayData();
    drawLastMonthDiagram();
}

void GoalsView::updateWeeklyGoal() {
    applicationSettings.setWeeklyPomodorosGoal(unsigned(ui->spinBoxWeeklyGoal->value()));
    displayData();
    drawLastQuarterDiagram();
}

void GoalsView::updateMonthlyGoal() {
    applicationSettings.setMonthlyPomodorosGoal(unsigned(ui->spinBoxMonthlyGoal->value()));
    displayData();
    drawLastYearDiagram();
}

QString GoalsView::formatDecimal(double decimal) const {
    return QString("%1").arg(decimal, 2, 'f', 2, '0');
}
