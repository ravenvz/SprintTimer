#ifndef GAUGE_H
#define GAUGE_H

#include <QWidget>

class Gauge : public QWidget
{
    Q_OBJECT
public:
    explicit Gauge(QWidget* parent = 0);
protected:
    void paintEvent(QPaintEvent*);
};



#endif
