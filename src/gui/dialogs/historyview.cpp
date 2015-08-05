#include "historyview.h"
#include "ui_history.h"


HistoryView::HistoryView(QDialog* parent) : 
    QDialog(parent),
    ui(new Ui::HistoryView)
{
    ui->setupUi(this);
}

HistoryView::~HistoryView() {
    delete ui;
}
