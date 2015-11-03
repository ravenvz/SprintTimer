#ifndef POMODORO_PICKPERIODWIDGET_H
#define POMODORO_PICKPERIODWIDGET_H

#include <src/gui/dialogs/datepickdialog.h>

namespace Ui {
    class PickPeriodWidget;
}


class PickPeriodWidget : public QWidget {

    Q_OBJECT
        
public:
    explicit PickPeriodWidget(QWidget* parent = 0);
    ~PickPeriodWidget();
    void setInterval(DateInterval interval);
    DateInterval getInterval() const;

signals:
    void intervalChanged(DateInterval newInterval);

private slots:
    void openDatePickDialog();
    void updateInterval();
    void updateInterval(DateInterval interval);
    void updateSelectionHintLabel();

private:
    Ui::PickPeriodWidget* ui;
    DateInterval selectedInterval;

    void connectSlots();

};


#endif //POMODORO_PICKPERIODWIDGET_H
