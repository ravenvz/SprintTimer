#include "TaskView.h"

TaskView::TaskView(QWidget* parent)
    : QListView(parent)
{
    taskViewDelegate = new TaskViewDelegate(this);
    setItemDelegate(taskViewDelegate);

    connect(this,
            &QListView::customContextMenuRequested,
            this,
            &TaskView::showTaskContextMenu);
}

void TaskView::dropEvent(QDropEvent* event)
{
    int rowMovedFrom = currentIndex().row();
    int rowMovedTo = indexAt(event->pos()).row();
    model()->moveRows(
        QModelIndex(), rowMovedFrom, 1, QModelIndex(), rowMovedTo);
}

void TaskView::setModels(TaskModel* taskModel, TagModel* tagModel)
{
    setModel(taskModel);
    this->tagModel = tagModel;
}

void TaskView::editTask()
{
    if (tagModel == nullptr) {
        // TODO log error
        qDebug() << "Passing null tagModel to TaskView";
        return;
    }
    QModelIndex index = currentIndex();
    // TODO provide proper implementation of data() and setData() in
    // TaskModel, so that dynamic casts like that could be avoided
    Task itemToEdit
        = dynamic_cast<TaskModel*>(model())->itemAt(index.row());
    AddTaskDialog dialog{tagModel};
    dialog.setWindowTitle("Edit Task");
    dialog.fillItemData(itemToEdit);
    if (dialog.exec()) {
        Task updatedItem = dialog.constructedTask();
        updatedItem.setSpentPomodoros(itemToEdit.spentPomodoros());
        updatedItem.setCompleted(itemToEdit.isCompleted());
        dynamic_cast<TaskModel*>(model())->replaceItemAt(index.row(),
                                                             updatedItem);
    }
}

void TaskView::removeTask()
{
    QModelIndex index = currentIndex();
    ConfirmationDialog dialog;
    // TODO figure out a way to handle this situation more gracefully
    QString description;
    if (dynamic_cast<TaskModel*>(model())
            ->itemAt(index.row())
            .spentPomodoros()
        > 0) {
        description
            = "WARNING! This todo item has pomodoros associated with it "
              "and they will be removed permanently along with this item.";
    }
    else {
        description = "This will delete todo item permanently!";
    }
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        dynamic_cast<TaskModel*>(model())->remove(index);
    }
}

void TaskView::launchTagEditor()
{
    if (!tagEditor) {
        tagEditor = new TagEditorWidget{tagModel};
        tagEditor->show();
    }
    else {
        tagEditor->raise();
        tagEditor->activateWindow();
        tagEditor->showNormal();
    }
}

void TaskView::showTaskContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu taskMenu;
    // Note QMenu takes ownership of Action
    taskMenu.addAction("Edit");
    taskMenu.addAction("Delete");
    taskMenu.addAction("Tag editor");

    QAction* selectedItem = taskMenu.exec(globalPos);

    if (selectedItem && selectedItem->text() == "Edit")
        editTask();
    if (selectedItem && selectedItem->text() == "Delete")
        removeTask();
    if (selectedItem && selectedItem->text() == "Tag editor")
        launchTagEditor();
}
