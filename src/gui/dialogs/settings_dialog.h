#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H


#include <QDialog>
#include <QSettings>
#include <src/core/config.h>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Config &applicationSettings, QDialog *parent = 0);
    ~SettingsDialog();
    void fillSettingsData();

private slots:
    void storeSettingsData();
    void toggleVolumeControlVisibility();

private:
    Ui::SettingsDialog *ui;
    Config& applicationSettings;

};

#endif // SETTINGSDIALOG_H
