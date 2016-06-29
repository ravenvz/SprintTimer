#include "widgets/PomodoroView.h"

PomodoroView::PomodoroView(QWidget* parent)
    : QListView{parent}
{
    connect(this,
            &QListView::customContextMenuRequested,
            this,
            &PomodoroView::showContextMenu);
}

void PomodoroView::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu pomodoroMenu;
    pomodoroMenu.addAction("Delete");

    QAction* selectedItem = pomodoroMenu.exec(globalPos);

    if (selectedItem && selectedItem->text() == "Delete")
        removePomodoro();
}

void PomodoroView::removePomodoro()
{
    QModelIndex index = currentIndex();
    ConfirmationDialog dialog;
    QString description{"This will remove pomodoro permanently"};
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        dynamic_cast<PomodoroModel*>(model())->remove(index.row());
    }
}
