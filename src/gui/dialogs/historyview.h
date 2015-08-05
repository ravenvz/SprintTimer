#ifndef HISTORY_VIEW_H
#define HISTORY_VIEW_H 

#include <QObject>
#include <QDialog>

namespace Ui {
    class HistoryView;
}

class HistoryView : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryView(QDialog* parent = 0);
    ~HistoryView();

private:
    Ui::HistoryView *ui;
};

#endif //HISTORY_VIEW_H 
