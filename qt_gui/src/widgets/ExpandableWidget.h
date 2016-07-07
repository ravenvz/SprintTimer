#ifndef EXPANDABLEWIDGET_H_9NQZP3TA
#define EXPANDABLEWIDGET_H_9NQZP3TA

#include <QWidget>
#include <memory>

class ExpansionState;

namespace Ui {
class ExpandableWidget;
} /* Ui */

class ExpandableWidget : public QWidget {

    Q_OBJECT

    friend class ExpansionState;
    friend class ExpandedFully;
    friend class Shrinked;
    friend class ExpandedMenuOnly;
    friend class ExpandedWithoutMenu;

public:
    explicit ExpandableWidget(QWidget* parent = nullptr);
    ~ExpandableWidget();
    QSize sizeHint() const override;

private:
    Ui::ExpandableWidget* ui;
    std::unique_ptr<ExpansionState> expandedFully;
    std::unique_ptr<ExpansionState> shrinked;
    std::unique_ptr<ExpansionState> expandedMenuOnly;
    std::unique_ptr<ExpansionState> expandedWithoutMenu;
    ExpansionState* expansionState;

    void setStateUi();

private slots:
    void toggleView();
    void toggleMenu();
};


class ExpansionState {
public:
    ExpansionState(int width, int height, ExpandableWidget& widget);
    ~ExpansionState() = default;
    QSize sizeHint() const;
    virtual void setStateUi() = 0;
    virtual void toggleView() = 0;
    virtual void toggleMenu() = 0;

protected:
    const int width;
    const int height;
    ExpandableWidget& widget;
};


class ExpandedFully final : public ExpansionState {
public:
    ExpandedFully(ExpandableWidget& widget);
    void setStateUi();
    void toggleView();
    void toggleMenu();
};


class Shrinked final : public ExpansionState {
public:
    Shrinked(ExpandableWidget& widget);
    void setStateUi();
    void toggleView();
    void toggleMenu();
};


class ExpandedMenuOnly final : public ExpansionState {
public:
    ExpandedMenuOnly(ExpandableWidget& widget);
    void setStateUi();
    void toggleView();
    void toggleMenu();
};


class ExpandedWithoutMenu final : public ExpansionState {
public:
    ExpandedWithoutMenu(ExpandableWidget& widget);
    void setStateUi();
    void toggleView();
    void toggleMenu();
};


#endif /* end of include guard: EXPANDABLEWIDGET_H_9NQZP3TA */
