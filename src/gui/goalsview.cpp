#include "goalsview.h"
#include "ui_goalsview.h"


GoalsView::GoalsView(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::GoalsView)
{
    ui->setupUi(this);
}

GoalsView::~GoalsView() {
    delete ui;
}
