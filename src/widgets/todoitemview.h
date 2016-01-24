#ifndef TODOITEMVIEW_H
#define TODOITEMVIEW_H

#include <QListView>
#include <QDebug>


class TodoItemView : public QListView
{
public:
    explicit TodoItemView(QWidget* parent = 0) :
        QListView(parent)
    {

    }

    // Overriding to catch dropEvent and prevent model start removing rows
    // attempting drag and drop by calling another method instead.
    void dropEvent(QDropEvent* event) override {
        int rowMovedFrom = currentIndex().row();
        int rowMovedTo = indexAt(event->pos()).row();
        model()->moveRows(QModelIndex(), rowMovedFrom, 1, QModelIndex(), rowMovedTo);
    }

};



#endif /* end of include guard: TODOITEMVIEW_H */
