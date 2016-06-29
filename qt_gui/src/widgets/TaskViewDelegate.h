#ifndef TASKVIEWDELEGATE_H_Z8NDXQM6
#define TASKVIEWDELEGATE_H_Z8NDXQM6


#include <QStyledItemDelegate>


class TaskViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TaskViewDelegate(QObject* parent);

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

#endif /* end of include guard: TASKVIEWDELEGATE_H_Z8NDXQM6 */
