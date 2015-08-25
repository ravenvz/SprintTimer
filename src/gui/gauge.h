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
};



#endif
