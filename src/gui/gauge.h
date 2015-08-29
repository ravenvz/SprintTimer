#ifndef GAUGE_H
#define GAUGE_H

#include <QWidget>

class Gauge : public QWidget
{
    Q_OBJECT
public:
    explicit Gauge(unsigned actual, unsigned goal, QWidget* parent = 0);
protected:
    void paintEvent(QPaintEvent*);
private:
    unsigned actual = 0;
    unsigned goal = 0;
    const QColor normalWork = QColor("#6baa15");
    const QColor overwork = Qt::red;
    const QColor noWork = Qt::gray;
    const QRectF size = QRectF(10, 10, 30, 30);
    const QRectF gaugeSize = QRectF(9.5, 9.5, 29.5, 29.5);
    const int offsetToTop = 90 * 16;
    const int fullCircle = 360 * 16;
};


#endif
