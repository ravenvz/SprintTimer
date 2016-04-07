#ifndef POMODORO_TAGDIAGRAM_H
#define POMODORO_TAGDIAGRAM_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>
#include "widgets/PieChart.h"
#include <experimental/optional>

using std::experimental::optional;


using TagCount = std::pair<std::string, double>;

class LegendLabel;

class TopTagDiagram : public QWidget {
    Q_OBJECT

public:
    explicit TopTagDiagram(QWidget* parent = 0);
    ~TopTagDiagram();
    void setData(const std::vector<IStatisticalChart::LabelValuePair>& data);
    void setLegendTitle(const QString& title);
    void setFont(QFont font);

private slots:
    void onPieSliceClicked(size_t sliceIndex);
    void onLegendItemClicked(size_t itemIndex);

signals:
    void sliceSelectionChanged(size_t newTagCountIndex);

private:
    IStatisticalChart* diagram;
    std::vector<LegendLabel*> labels;
    QVBoxLayout* legendLayout;
    QLabel* labelLegendTitle;
    optional<size_t> selectedSliceIndex;

    void addLegendLabels(const std::vector<TagCount>& data);
};

class LegendLabel : public QLabel {
    Q_OBJECT

public:
    LegendLabel(const QString& text, size_t itemIndex, QWidget* parent = 0);
    void toggleSelected();
    bool isSelected() const;
    ~LegendLabel();

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    size_t itemIndex;
    bool selected{false};

signals:
    void clicked(size_t itemIndex);
};


#endif // POMODORO_TAGDIAGRAM_H
