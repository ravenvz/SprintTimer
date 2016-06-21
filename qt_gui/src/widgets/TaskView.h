#ifndef TASKVIEW_H_KN6BYIAY
#define TASKVIEW_H_KN6BYIAY


#include "dialogs/AddTaskDialog.h"
#include "dialogs/confirmationdialog.h"
#include "models/TagModel.h"
#include "models/TaskModel.h"
#include "widgets/TaskViewDelegate.h"
#include "widgets/tageditorwidget.h"
#include <QDropEvent>
#include <QListView>
#include <QMenu>
#include <QPointer>


class TaskView : public QListView {

public:
    explicit TaskView(QWidget* parent);

    // Overriding to catch dropEvent and prevent model start removing rows
    // attempting drag and drop by calling another method instead.
    void dropEvent(QDropEvent* event) final;

    void setModels(TaskModel* taskModel, TagModel* tagModel);

private:
    QPointer<TaskViewDelegate> taskViewDelegate;
    TagModel* tagModel;
    QPointer<TagEditorWidget> tagEditor;

    void editTask();

    void removeTask();

    void launchTagEditor();

public slots:

    void showTaskContextMenu(const QPoint& pos);
};


#endif /* end of include guard: TASKVIEW_H_KN6BYIAY */
