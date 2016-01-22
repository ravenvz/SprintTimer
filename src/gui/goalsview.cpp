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
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->spinBoxDailyGoal->setValue(applicationSettings.getDailyPomodorosGoal());
    ui->spinBoxWeeklyGoal->setValue(applicationSettings.getWeeklyPomodorosGoal());
    ui->spinBoxMonthlyGoal->setValue(applicationSettings.getMonthlyPomodorosGoal());
    displayData();
    connect(ui->spinBoxDailyGoal, SIGNAL(valueChanged(int)), this, SLOT(updateDailyGoal(int)));
    connect(ui->spinBoxWeeklyGoal, SIGNAL(valueChanged(int)), this, SLOT(updateWeeklyGoal(int)));
    connect(ui->spinBoxMonthlyGoal, SIGNAL(valueChanged(int)), this, SLOT(updateMonthlyGoal(int)));
}

GoalsView::~GoalsView() {
    delete ui;
}

void GoalsView::updateView() {
    displayData();
}

void GoalsView::displayData() {
    displayDailyData();
    displayWeeklyData();
    displayMonthlyData();
}

void GoalsView::displayDailyData() {
    unsigned dailyGoal = applicationSettings.getDailyPomodorosGoal();
    unsigned monthlyGoal = applicationSettings.getMonthlyPomodorosGoal();
    Distribution<unsigned> lastDays {PomodoroDataSource::getPomodorosForLastThirtyDays()};
    ui->labelLastMonthAverage->setText(formatDecimal(lastDays.getAverage()));
    ui->labelLastMonthPercentage->setText(QString("%1%")
            .arg(formatDecimal(MathUtils::percentage(lastDays.getTotal(), monthlyGoal))));
    ui->labelTodayProgress->setText(QString("%1").arg(lastDays.getTotal()));
    updateProgressBar(ui->progressBarToday, dailyGoal, lastDays.getBinValue(lastDays.getNumBins() - 1));
    drawPeriodDiagram(ui->gridLayoutLastMonthDiagram, lastDays, dailyGoal, 3, 10);
}

void GoalsView::displayWeeklyData() {
    unsigned weeklyGoal = applicationSettings.getWeeklyPomodorosGoal();
    Distribution<unsigned> lastWeeks {PomodoroDataSource::getPomodorosForLastTwelveWeeks()};
    ui->labelLastQuarterAverage->setText(formatDecimal(lastWeeks.getAverage()));
    ui->labelLastQuarterPercentage->setText(QString("%1%")
            .arg(formatDecimal(MathUtils::percentage(lastWeeks.getTotal(), lastWeeks.getNumBins() * weeklyGoal))));
    ui->labelWeekProgress->setText(QString("%1").arg(lastWeeks.getTotal()));
    updateProgressBar(ui->progressBarWeek, weeklyGoal, lastWeeks.getBinValue(lastWeeks.getNumBins() - 1));
    drawPeriodDiagram(ui->gridLayoutLastQuarterDiagram, lastWeeks, weeklyGoal, 3, 4);
}

void GoalsView::displayMonthlyData() {
    unsigned monthlyGoal = applicationSettings.getMonthlyPomodorosGoal();
    Distribution<unsigned> lastMonths {PomodoroDataSource::getPomodorosForLastTwelveMonths()};
    ui->labelLastYearAverage->setText(formatDecimal(lastMonths.getAverage()));
    ui->labelLastYearPercentage->setText(QString("%1%")
            .arg(formatDecimal(MathUtils::percentage(lastMonths.getTotal(), lastMonths.getNumBins() * monthlyGoal))));
    ui->labelMonthProgress->setText(QString("%1").arg(lastMonths.getTotal()));
    updateProgressBar(ui->progressBarMonth, monthlyGoal, lastMonths.getBinValue(
            lastMonths.getNumBins() - 1));
    drawPeriodDiagram(ui->gridLayoutLastYearDiagram, lastMonths, monthlyGoal, 3, 4);
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

void GoalsView::drawPeriodDiagram(QGridLayout* layout, Distribution<unsigned>& distribution,
        unsigned goal, int rowNum, int colNum) {
    clearDiagramLayout(layout);
    GaugeFactory factory;
    for (int row = 0, ind = 0; row < rowNum; ++row) {
        for (int col = 0; col < colNum; ++col, ++ind) {
            layout->addWidget(
                    factory.create(distribution.getBinValue(ind), goal, this), row, col);
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

void GoalsView::updateDailyGoal(int newValue) {
    applicationSettings.setDailyPomodorosGoal(static_cast<unsigned>(newValue));
    displayDailyData();
}

void GoalsView::updateWeeklyGoal(int newValue) {
    applicationSettings.setWeeklyPomodorosGoal(static_cast<unsigned>(newValue));
    displayWeeklyData();
}

void GoalsView::updateMonthlyGoal(int newValue) {
    applicationSettings.setMonthlyPomodorosGoal(static_cast<unsigned>(newValue));
    displayMonthlyData();
}

QString GoalsView::formatDecimal(double decimal) const {
    return QString("%1").arg(decimal, 2, 'f', 2, '0');
}
