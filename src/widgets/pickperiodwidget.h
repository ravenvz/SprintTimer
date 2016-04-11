#ifndef POMODORO_PICKPERIODWIDGET_H
#define POMODORO_PICKPERIODWIDGET_H

#include <QPointer>
#include "dialogs/datepickdialog.h"

namespace Ui {
    class PickPeriodWidget;
}


class PickPeriodWidget : public QWidget {

    Q_OBJECT
        
public:
    explicit PickPeriodWidget(QWidget* parent = 0);
    ~PickPeriodWidget();
    void setInterval(DateInterval timeSpan);
    DateInterval getInterval() const;
    void setYears(const QStringList& years);

signals:
    void timeSpanChanged(DateInterval newInterval);

private slots:
    void openDatePickDialog();
    void updateInterval();
    void updateInterval(DateInterval timeSpan);
    void updateSelectionHintLabel();

private:
    Ui::PickPeriodWidget* ui;
    DateInterval selectedInterval;
    QPointer<QStringListModel> yearsModel;
    QPointer<QStringListModel> monthsModel;

    void connectSlots();

};


#endif //POMODORO_PICKPERIODWIDGET_H
