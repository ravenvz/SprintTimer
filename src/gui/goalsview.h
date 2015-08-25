#ifndef GOALSVIEW_H
#define GOALSVIEW_H

#include <QWidget>
#include "core/config.h"

namespace Ui {
    class GoalsView;
}

class GoalsView : public QWidget
{
    Q_OBJECT

public:
    explicit GoalsView(Config& applicationSettings, QWidget* parent = 0);
    ~GoalsView();

private slots:
    void updateDailyGoal();
    void updateWeeklyGoal();
    void updateMonthlyGoal();

private:
    Ui::GoalsView* ui;
    Config& applicationSettings;

    void connectSlots();
    void drawDiagrams();
};


#endif
