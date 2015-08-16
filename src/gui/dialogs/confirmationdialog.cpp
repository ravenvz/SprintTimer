#include "confirmationdialog.h"
#include "ui_pom_cancel_dialog.h"

ConfirmationDialog::ConfirmationDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ConfirmationDialog)
{
    ui->setupUi(this);
}

ConfirmationDialog::~ConfirmationDialog()
{
    delete ui;
}

void ConfirmationDialog::setActionDescription(QString& description) {
    ui->lblDescribeAction->setText(description);
}
