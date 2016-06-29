#ifndef POMODOROVIEW_H_TMGU1KAJ
#define POMODOROVIEW_H_TMGU1KAJ

#include "dialogs/confirmationdialog.h"
#include "models/PomodoroModel.h"
#include <QListView>
#include <QMenu>

class PomodoroView : public QListView {
public:
    PomodoroView(QWidget* parent);

private:
    void removePomodoro();

private slots:
    void showContextMenu(const QPoint& pos);
};

#endif /* end of include guard: POMODOROVIEW_H_TMGU1KAJ */
