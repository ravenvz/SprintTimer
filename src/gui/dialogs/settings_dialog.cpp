#include "settings_dialog.h"
#include "ui_settings.h"
// #include "gui/settings_window.h"


SettingsDialog::SettingsDialog(QDialog* parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}
