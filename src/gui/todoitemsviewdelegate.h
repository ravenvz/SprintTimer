#ifndef TODOITEMSVIEWDELEGATE_H
#define TODOITEMSVIEWDELEGATE_H


#include <QStyledItemDelegate>


class TodoItemsViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    TodoItemsViewDelegate(QObject* parent = 0);
    ~TodoItemsViewDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    static QSize iconSize;
    static int padding;
};

#endif // TODOITEMSVIEWDELEGATE_H
