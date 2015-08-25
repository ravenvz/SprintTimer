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
    QVector<unsigned> lastThirty = PomodoroGateway::getNumCompletedPomodorosForLastThirtyDays();
    QVector<unsigned> lastQuarter = PomodoroGateway::getCompletedPomodorosDistributionForLastThreeMonths();
    QVector<unsigned> lastYear = PomodoroGateway::getCompletedPomodorosDistributionForLastTwelveMonths();
    unsigned dailyGoal = applicationSettings.getDailyPomodorosGoal();
    unsigned weeklyGoal = applicationSettings.getWeeklyPomodorosGoal();
    unsigned monthlyGoal = applicationSettings.getMonthlyPomodorosGoal();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 10; ++j) {
            ui->gridLayoutLastMonthDiagram->addWidget(new Gauge(12, dailyGoal, this), i, j);
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ui->gridLayoutLastQuarterDiagram->addWidget(new Gauge(10, weeklyGoal, this), i, j);
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ui->gridLayoutLastYearDiagram->addWidget(new Gauge(79, monthlyGoal, this), i, j);
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
