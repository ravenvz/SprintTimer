#ifndef GAUGE_H
#define GAUGE_H

#include <QWidget>
#include <QEvent>

#include <QDebug>


// TODO when final decision will be made on how gauges should look
// like in different states, Gauge class hierarhy should be reviewed
// (and possibly even be reduced to single class). Until then, having
// hierarhy and factory makes sence, as it simplifies experimenting.


class Gauge : public QWidget {
    Q_OBJECT

public:
    Gauge(int actual, int goal, QWidget* parent = 0);
    virtual ~Gauge() = default;

protected:
    int actual;
    int goal;
    bool displayDetails = false;
    bool sizesComputed = false;
    QRectF outerRect;
    QRectF innerRect;
    QColor filled = QColor("#6baa15");
    QColor empty = Qt::gray;
    QColor overfilled = Qt::red; // TODO not being used right now
    const int offsetToTop = 90 * 16;

    void paintEvent(QPaintEvent*) override;
    bool eventFilter(QObject* object, QEvent* event) override;
    void computeAdaptiveSizes();
    void setupPainter(QPainter& painter);
    virtual void drawOuterCircle(QPainter& painter);
    virtual void drawInnerCircle(QPainter& painter);
};


class OverfilledGauge : public Gauge {
public:
    OverfilledGauge(int actual, int goal, QWidget* parent = 0);
};


class EmptyGauge : public Gauge {
public:
    EmptyGauge(int actual, int goal, QWidget* parent = 0);

protected:
    void drawOuterCircle(QPainter& painter) override;
};


class FilledGauge : public Gauge {
public:
    FilledGauge(int actual, int goal, QWidget* parent = 0);

protected:
    void drawOuterCircle(QPainter& painter) override;
};


class GaugeFactory {

public:
    GaugeFactory() {}

    Gauge* create(int filled, int total, QWidget* parent = 0)
    {
        Gauge* ptr;
        if (filled == 0 || total == 0) {
            ptr = new EmptyGauge(filled, total, parent);
        }
        else if (filled == total) {
            ptr = new FilledGauge(filled, total, parent);
        }
        else if (filled > total) {
            ptr = new OverfilledGauge(filled, total, parent);
        }
        else {
            ptr = new Gauge(filled, total, parent);
        }
        return ptr;
    }
};

#endif
