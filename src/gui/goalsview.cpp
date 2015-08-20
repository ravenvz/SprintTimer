#include "goalsview.h"
#include "ui_goalsview.h"
#include "gauge.h"


GoalsView::GoalsView(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::GoalsView)
{
    ui->setupUi(this);
    drawDiagrams();
}

GoalsView::~GoalsView() {
    delete ui;
}

void GoalsView::drawDiagrams() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 10; ++j) {
            ui->gridLayoutLastMonthDiagram->addWidget(new Gauge(this), i, j);
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ui->gridLayoutLastQuarterDiagram->addWidget(new Gauge(this), i, j);
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ui->gridLayoutLastYearDiagram->addWidget(new Gauge(this), i, j);
        }
    }

}
