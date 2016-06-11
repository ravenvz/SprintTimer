#ifndef IDATAWIDGET_H_DMUO9TSG
#define IDATAWIDGET_H_DMUO9TSG

#include <QWidget>

/* Widget that displays some data (like statistics, etc) and has
 * means to access this data, but does not now when underlying data
 * has been changed.
 *
 * Extends QWidget to provide 'synchronize()' slot.
 * It's ment to be called when widget's underlying data
 * has been changed and should be requeried.*/
class DataWidget : public QWidget {
    Q_OBJECT

public:
    DataWidget(QWidget* parent);

    virtual ~DataWidget() = default;

public slots:
    virtual void synchronize() = 0;
};

#endif /* end of include guard: IDATAWIDGET_H_DMUO9TSG */
