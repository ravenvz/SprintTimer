#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H


#include "core/IConfig.h"
#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(IConfig& applicationSettings,
                            QDialog* parent = nullptr);
    ~SettingsDialog();
    void fillSettingsData();

private slots:
    void storeSettingsData();
    void toggleVolumeControlVisibility();
    void onBrowseSoundFileButtonClicked();

private:
    Ui::SettingsDialog* ui;
    IConfig& applicationSettings;
};

#endif // SETTINGSDIALOG_H
