#ifndef TODOITEMSLISTVIEW_H
#define TODOITEMSLISTVIEW_H

#include <QListView>


class TodoItemsListView : public QListView
{
public:
    TodoItemsListView(QWidget* centralWidget);
    // void dragEnterEvent(QDragEnterEvent* event) override;
    // void dragMoveEvent(QDragMoveEvent* event) override;
    // void dropEvent(QDropEvent* event) override;
};

#endif // TODOITEMSLISTVIEW_H
