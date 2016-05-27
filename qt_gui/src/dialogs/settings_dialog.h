#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H


#include <QDialog>
#include <QSettings>
#include "core/IConfig.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(IConfig& applicationSettings, QDialog* parent = 0);
    ~SettingsDialog();
    void fillSettingsData();

private slots:
    void storeSettingsData();
    void toggleVolumeControlVisibility();

private:
    Ui::SettingsDialog* ui;
    IConfig& applicationSettings;
};

#endif // SETTINGSDIALOG_H
