#include "pomodorocanceldialog.h"
#include "ui_pom_cancel_dialog.h"

PomodoroCancelDialog::PomodoroCancelDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::PomodoroCancelDialog)
{
    ui->setupUi(this);
}

PomodoroCancelDialog::~PomodoroCancelDialog()
{
    delete ui;
}

void PomodoroCancelDialog::setActionDescription(QString& description) {
    ui->lblDescribeAction->setText(description);
}
