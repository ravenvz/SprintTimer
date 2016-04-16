#ifndef GAUGE_H
#define GAUGE_H

#include <QEvent>
#include <QWidget>
#include <memory>


class HoverState;
class HoverStateHovered;
class HoverStateUnhovered;

class WorkProgressState;
class WorkProgressNone;
class WorkProgressUnderwork;
class WorkProgressDone;
class WorkProgressOverwork;

class Gauge : public QWidget {
    Q_OBJECT

public:
    Gauge(int actual, int goal, QWidget* parent = 0);
    void setData(int actual, int goal);
    virtual ~Gauge() = default;

protected:
    std::unique_ptr<HoverState> hoveredState;
    std::unique_ptr<HoverState> unhoveredState;
    std::unique_ptr<WorkProgressState> workProgressUnderwork;
    std::unique_ptr<WorkProgressState> workProgressOverwork;
    std::unique_ptr<WorkProgressState> workProgressNone;
    std::unique_ptr<WorkProgressState> workProgressDone;
    HoverState* hoverState;
    WorkProgressState* workProgressState;
    int actual{0};
    int goal{0};
    bool sizesComputed = false;
    QRectF outerRect;
    QRectF innerRect;

    void paintEvent(QPaintEvent*) override;
    bool eventFilter(QObject* object, QEvent* event) override;
    void computeAdaptiveSizes();
    void setupPainter(QPainter& painter);
    void drawOuterCircle(QPainter& painter);
    void drawInnerCircle(QPainter& painter);
};


class HoverState {
public:
    virtual ~HoverState() = default;

    virtual void
    draw(QPainter& painter, const QRectF& rect, int actual, int goal)
        = 0;

protected:
    virtual void
    drawText(QPainter& painter, const QRectF& rect, const QString& text);
};

class HoverStateHovered : public HoverState {
public:
    void
    draw(QPainter& painter, const QRectF& rect, int actual, int goal) final;
};

class HoverStateUnhovered : public HoverState {
public:
    void
    draw(QPainter& painter, const QRectF& rect, int actual, int goal) final;
};

class WorkProgressState {
public:
    virtual void
    draw(QPainter& painter, const QRectF& rect, int actual, int goal)
        = 0;
    virtual ~WorkProgressState() = default;

protected:
    virtual void draw(QPainter& painter,
                      const QRectF& rect,
                      int actual,
                      int goal,
                      const QBrush& empty,
                      const QBrush& filled);
};

class WorkProgressUnderwork : public WorkProgressState {
public:
    void
    draw(QPainter& painter, const QRectF& rect, int actual, int goal) final;
};

class WorkProgressOverwork : public WorkProgressState {
public:
    void
    draw(QPainter& painter, const QRectF& rect, int actual, int goal) final;
};

class WorkProgressDone : public WorkProgressState {
public:
    void
    draw(QPainter& painter, const QRectF& rect, int actual, int goal) final;
};

class WorkProgressNone : public WorkProgressState {
public:
    void
    draw(QPainter& painter, const QRectF& rect, int actual, int goal) final;
};

class GaugeFactory {

public:
    GaugeFactory() {}

    Gauge* create(int filled, int total, QWidget* parent = 0)
    {
        return new Gauge(filled, total, parent);
    }
};

#endif
