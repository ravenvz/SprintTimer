#ifndef POMOTIMEDIAGRAM_H
#define POMOTIMEDIAGRAM_H

#include "core/TimeSpan.h"
#include <QEvent>
#include <QWidget>
#include <vector>


/* Displays vector of TimeSpan as a diagram.
 *
 * Diagram looks like a round clock that can display 24-hours instead
 * of 12 with resolution of 1 hour.
 * Top point corresponds to midnight, right point to 6:00,
 * bottom point to 12:00, left point to 18:00.
 *
 * Time spans are painted as a sector on this diagram. If multiple time spans
 * are intersecting, intersection is painted in brighter color.
 */
class TimeDiagram : public QWidget {
    Q_OBJECT

public:
    explicit TimeDiagram(QWidget* parent);

    ~TimeDiagram();

    /* Set vector of time intervals to display as a diagram. */
    void setIntervals(std::vector<TimeSpan> newIntervals);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    const QColor timeSpanColor = QColor::fromRgb(246, 61, 13, 20);
    std::vector<TimeSpan> timeSpans;
    QRectF totalSizeRect;
    QRectF diagramRect;
    double diagramRadius{0};
    double tickOffset{0};
    double longTickLength{0};
    double shortTickLength{0};

    void computeAdaptiveSizes();

    void drawDiagramCanvas(QPainter& painter);

    void drawIntervals(QPainter& painter);
};

#endif /* end of include guard: POMOTIMEDIAGRAM_H */
