#ifndef POMODORO_TAGDIAGRAM_H
#define POMODORO_TAGDIAGRAM_H


#include <QWidget>


class PieDiagram : public QWidget
{
    Q_OBJECT

public:
    explicit PieDiagram(int numSlices, QWidget* parent = 0);
    void setData(QHash<QString, unsigned> data);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    QRectF totalSizeRect;
    QRectF expandedSliceRect;
    QRectF diagramRect;
    double expandedShiftLength;
    QHash<QString, unsigned> dataHash;
    const int numSlices;

    void computeAdaptiveSizes();
    QPointF computeOffsetPoint(double current, double offset);
    QVector<double> computeAngles() const;
};


#endif //POMODORO_TAGDIAGRAM_H
