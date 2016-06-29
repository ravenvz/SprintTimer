#ifndef BARCHART_H
#define BARCHART_H

#include <QEvent>
#include <QPen>
#include <QWidget>
#include <vector>

struct BarDataItem {
    QString label;
    double value;
    double normalizedValue;
};

class BarData {

public:
    BarData();

    /* Both vectors should be of equal size, otherwise data would
     * be set only for number of bars equal to shortest vector */
    // BarData(std::vector<double>& values, std::vector<QString>& labels);
    BarData(const std::vector<double>& values,
            const std::vector<QString>& labels);

    const BarDataItem& operator[](size_t idx) const;
    size_t size() const;

private:
    std::vector<BarDataItem> data;

    void normalize();
};

class BarChart : public QWidget {
    Q_OBJECT

public:
    explicit BarChart(QWidget* parent);
    void setData(const BarData& data);
    void setPen(QPen& barPen);
    void setBrush(QBrush& barBrush);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    BarData barData;
    QPen pen;
    QBrush brush;
    QRectF availableRect;
    std::vector<QRectF> barRectangles;

    void computeAdaptiveSizes();
    void drawBars(QPainter& painter);
};

#endif // BARCHART_H
