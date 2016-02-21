#ifndef BARCHART_H
#define BARCHART_H


#include <QWidget>
#include <QEvent>
#include <QPen>


struct BarDataItem
{
    QString label;
    double value;
    double normalizedValue;
};


class BarData
{

public:
    BarData();
    /* Both vectors should be of equal size, otherwise data would
     * be set only for number of bars equal to shortest vector */
    BarData(QVector<double>& values, QVector<QString>& labels);
    const BarDataItem& operator[](std::size_t idx) const;
    int size() const;

private:
    QVector<BarDataItem> data;

    void normalize();
};


class BarChart : public QWidget
{
    Q_OBJECT

public:
    explicit BarChart(QWidget* parent = 0);
    void setData(const BarData& data);
    void setPen(QPen& barPen);
    void setBrush(QBrush& barBrush);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    BarData barData;
    QPen pen;
    QBrush brush;
    QRectF availableRect;
    QVector<QRectF> barRectangles;

    void computeAdaptiveSizes();
    void drawBars(QPainter& painter);

};



#endif // BARCHART_H
