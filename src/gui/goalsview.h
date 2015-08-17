#ifndef GOALSVIEW_H
#define GOALSVIEW_H

#include <QWidget>

namespace Ui {
    class GoalsView;
}

class GoalsView : public QWidget
{
    Q_OBJECT

public:
    explicit GoalsView(QWidget* parent = 0);
    ~GoalsView();

private:
    Ui::GoalsView* ui;
};


#endif
