#ifndef POMODORO_TAGDIAGRAM_H
#define POMODORO_TAGDIAGRAM_H


#include <QWidget>
#include <QMouseEvent>


class PieDiagram : public QWidget
{
    Q_OBJECT

public:
    explicit PieDiagram(int numSlices, QWidget* parent = 0);
    void setData(QHash<QString, unsigned> data);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent * event) override;

private:
    QRectF totalSizeRect;
    QRectF expandedSliceRect;
    QRectF diagramRect;
    double expandedShiftLength;
    QHash<QString, unsigned> dataHash;
    const int numSlices;
    QVector<double> angles;
    int selectedPieIndex = -1;

    void connectSlots();
    void computeAdaptiveSizes();
    QPointF computeOffsetPoint(double current, double offset);
    void computeAngles();
    void updateSelectedSliceIndex(const QPoint& pos);

private slots:
    void onSliceSelectionChanged(int sliceIndex);

signals:
    void sliceSelectionChanged(int newSliceIndex);

};


#endif //POMODORO_TAGDIAGRAM_H
