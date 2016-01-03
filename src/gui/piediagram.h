#ifndef POMODORO_TAGDIAGRAM_H
#define POMODORO_TAGDIAGRAM_H


#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>


typedef std::pair<QString, double> Slice;
class Diagram;
class LegendLabel;


class PieDiagram : public QWidget
{
    Q_OBJECT

public:
    PieDiagram(QWidget* parent = 0);
    ~PieDiagram();
    void setData(QVector<Slice>& data);
    void setLegendTitle(QString title);
    void setFont(QFont font);

private:
    Diagram* diagram;
    QVector<LegendLabel*> labels;
    QVBoxLayout* legendLayout;
    QLabel* labelLegendTitle;
    int selectedSlice = -1;


private slots:
    void onSliceSelectionChanged(int sliceIndex);
    void onLegendItemClicked(int itemIndex);

signals:
    void sliceSelectionChanged(int newSliceIndex);
};


class LegendLabel : public QLabel
{
    Q_OBJECT

public:
    LegendLabel(const QString& text, int itemIndex, QWidget* parent = 0);
    ~LegendLabel();

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    int itemIndex;

signals:
    void clicked(int itemIndex);

};


class Diagram : public QWidget
{
    Q_OBJECT

public:
    explicit Diagram(QWidget* parent = 0);
    void setData(QVector<Slice>& data);
    void setSelectedSlice(int sliceIndex);

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
    const double pi {acos(-1)};
    QVector<QBrush> brushes {QBrush(QColor("#28245a")),
                             QBrush(QColor("#73c245")),
                             QBrush(QColor("#ea6136")),
                             QBrush(QColor("#1d589b")),
                             QBrush(QColor("#d62a36")),
                             QBrush(QColor("#401b60")),
                             QBrush(QColor("#f8cd32")),
                             QBrush(QColor("#258bc8")),
                             QBrush(QColor("#087847"))};

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
