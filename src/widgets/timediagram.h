#ifndef POMOTIMEDIAGRAM_H
#define POMOTIMEDIAGRAM_H

#include <QVector>
#include <QWidget>
#include <QEvent>
#include "core/TaskScheduler.h"


class TimeDiagram : public QWidget
{
    Q_OBJECT

public:
    explicit TimeDiagram(QWidget* parent = 0);
    ~TimeDiagram();
    void setIntervals(QVector<TimeSpan> newIntervals);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    const QColor timeSpanColor = QColor::fromRgb(246, 61, 13, 20);
    QVector<TimeSpan> intervals;
    QRectF totalSizeRect;
    QRectF diagramRect;
    double diagramRadius {0};
    double tickOffset {0};
    double longTickLength {0};
    double shortTickLength {0};

    void computeAdaptiveSizes();
    void drawDiagramCanvas(QPainter& painter);
    void drawIntervals(QPainter& painter);
};

#endif /* end of include guard: POMOTIMEDIAGRAM_H */
