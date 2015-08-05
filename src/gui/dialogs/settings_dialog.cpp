#include <src/core/config.h>
#include "settings_dialog.h"
#include "ui_settings.h"


SettingsDialog::SettingsDialog(Config& applicationSettings, QDialog* parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(storeSettingsData()));
    connect(ui->chBxPlaySound, SIGNAL(stateChanged(int)), this, SLOT(toggleVolumeControlVisibility()));
}


SettingsDialog::~SettingsDialog() {
    delete ui;
}


void SettingsDialog::fillSettingsData() {
    ui->spBxPomodoroDuration->setValue(applicationSettings.getPomodoroDuration());
    ui->spBxShortDuration->setValue(applicationSettings.getShortBreakDuration());
    ui->spBxLongDuration->setValue(applicationSettings.getLongBreakDuration());
    ui->spBxLongBreakAfter->setValue(applicationSettings.getTasksBeforeBreak());
    ui->chBxPlaySound->setChecked(applicationSettings.soundIsEnabled());
    ui->hSliderVolume->setValue(applicationSettings.getSoundVolume());
}


void SettingsDialog::storeSettingsData() {
    applicationSettings.setPomodoroDuration(ui->spBxPomodoroDuration->value());
    applicationSettings.setShortBreakDuration(ui->spBxShortDuration->value());
    applicationSettings.setLongBreakDuration(ui->spBxLongDuration->value());
    applicationSettings.setTasksBeforeBreak(ui->spBxLongBreakAfter->value());
    applicationSettings.setPlaySound(ui->chBxPlaySound->isChecked());
    applicationSettings.setSoundVolume(ui->hSliderVolume->value());
}


void SettingsDialog::toggleVolumeControlVisibility() {
    ui->lblSoundVolume->setEnabled(ui->chBxPlaySound->isChecked());
    ui->hSliderVolume->setEnabled(ui->chBxPlaySound->isChecked());
}

