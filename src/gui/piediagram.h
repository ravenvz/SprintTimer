#ifndef POMODORO_TAGDIAGRAM_H
#define POMODORO_TAGDIAGRAM_H


#include <QWidget>
#include <QMouseEvent>

typedef std::pair<QString, double> Slice;


class PieDiagram : public QWidget
{
    Q_OBJECT

public:
    explicit PieDiagram(QWidget* parent = 0);
    void setData(QVector<Slice> data);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent * event) override;

private:
    QRectF totalSizeRect;
    QRectF expandedSliceRect;
    QRectF diagramRect;
    double expandedShiftLength;
    QVector<Slice> sortedData;
    int selectedPieIndex = -1;

    void connectSlots();
    void computeAdaptiveSizes();
    QPointF computeOffsetPoint(double current, double offset);
    void updateSelectedSliceIndex(const QPoint& pos);

private slots:
    void onSliceSelectionChanged(int sliceIndex);

signals:
    void sliceSelectionChanged(int newSliceIndex);

};


#endif //POMODORO_TAGDIAGRAM_H
