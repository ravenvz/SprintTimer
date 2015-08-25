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
    ui->spinBoxDailyGoal->setValue(applicationSettings.getDailyPomodorosGoal());
    ui->spinBoxWeeklyGoal->setValue(applicationSettings.getWeeklyPomodorosGoal());
    ui->spinBoxMonthlyGoal->setValue(applicationSettings.getMonthlyPomodorosGoal());
    lastThirty = PomodoroGateway::getNumCompletedPomodorosForLastThirtyDays();
    lastQuarter = PomodoroGateway::getCompletedPomodorosDistributionForLastThreeMonths();
    lastYear = PomodoroGateway::getCompletedPomodorosDistributionForLastTwelveMonths();
    connectSlots();
    drawDiagrams();
}

GoalsView::~GoalsView() {
    delete ui;
}

void GoalsView::connectSlots() {
    connect(ui->spinBoxDailyGoal, SIGNAL(valueChanged(int)), this, SLOT(updateDailyGoal()));
    connect(ui->spinBoxWeeklyGoal, SIGNAL(valueChanged(int)), this, SLOT(updateWeeklyGoal()));
    connect(ui->spinBoxMonthlyGoal, SIGNAL(valueChanged(int)), this, SLOT(updateMonthlyGoal()));
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
            qDebug() << ind;
            ui->gridLayoutLastQuarterDiagram->addWidget(new Gauge(lastQuarter.at(ind), weeklyGoal, this), row, col);
            ui->gridLayoutLastYearDiagram->addWidget(new Gauge(lastYear.at(ind), monthlyGoal, this), row, col);
        }
    }
}

void GoalsView::updateDailyGoal() {
    applicationSettings.setDailyPomodorosGoal(ui->spinBoxDailyGoal->value());
    qDebug() << "Here";
    drawDiagrams();
}

void GoalsView::updateWeeklyGoal() {
    applicationSettings.setWeeklyPomodorosGoal(ui->spinBoxWeeklyGoal->value());
    drawDiagrams();
}

void GoalsView::updateMonthlyGoal() {
    applicationSettings.setMonthlyPomodorosGoal(ui->spinBoxMonthlyGoal->value());
    drawDiagrams();
}
