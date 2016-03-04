#ifndef TODOITEMSVIEWDELEGATE_H
#define TODOITEMSVIEWDELEGATE_H


#include <QStyledItemDelegate>


class TodoItemsViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TodoItemsViewDelegate(QObject* parent = 0);
    ~TodoItemsViewDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    static constexpr int padding {1};
    static constexpr int verticalSpacing {14};

private:

    void paintText(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, const int alpha) const;
    
    QColor tagColor {Qt::blue};
    QColor commonColor {Qt::black};
    QColor overspentColor {Qt::red};
    QColor delimiterColor {Qt::gray};
    const int completedItemAlpha {100};
    const int normalItemAlpha {255};

};

#endif // TODOITEMSVIEWDELEGATE_H
