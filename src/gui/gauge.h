#ifndef GAUGE_H
#define GAUGE_H

#include <QWidget>
#include <QEvent>


class Gauge : public QWidget
{
    Q_OBJECT

public:
    explicit Gauge(unsigned actual, unsigned goal, QWidget* parent = 0);

protected:
    void paintEvent(QPaintEvent*) override;
    bool eventFilter(QObject* object, QEvent* event) override;

private:
    unsigned actual = 0;
    unsigned goal = 0;
    bool displayDetails = false;
    const QColor normalWork = QColor("#6baa15");
    const QColor overwork = Qt::red;
    const QColor noWork = Qt::gray;
    const QRectF size = QRectF(3, 3, 30, 30);
    const int offsetToTop = 90 * 16;
    const int fullCircle = 360 * 16;
};


#endif
