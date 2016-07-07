#include "widgets/ExpandableWidget.h"
#include "ui_expandable_widget.h"
#include <QDebug>

ExpandableWidget::ExpandableWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ExpandableWidget)
    , expandedFully{std::make_unique<ExpandedFully>(*this)}
    , shrinked{std::make_unique<Shrinked>(*this)}
    , expandedMenuOnly{std::make_unique<ExpandedMenuOnly>(*this)}
    , expandedWithoutMenu{std::make_unique<ExpandedWithoutMenu>(*this)}
    , expansionState{shrinked.get()}
{
    ui->setupUi(this);

    connect(ui->pbToggleView,
            &QPushButton::clicked,
            this,
            &ExpandableWidget::toggleView);
    connect(ui->pbToggleMenu,
            &QPushButton::clicked,
            this,
            &ExpandableWidget::toggleMenu);
    setStateUi();
}

ExpandableWidget::~ExpandableWidget() { delete ui; }

QSize ExpandableWidget::sizeHint() const { return expansionState->sizeHint(); }

void ExpandableWidget::toggleView()
{
    expansionState->toggleView();
    setStateUi();
}

void ExpandableWidget::toggleMenu()
{
    expansionState->toggleMenu();
    setStateUi();
}

void ExpandableWidget::setStateUi()
{
    expansionState->setStateUi();
    adjustSize();
}


ExpansionState::ExpansionState(int width, int height, ExpandableWidget& widget)
    : width{width}
    , height{height}
    , widget{widget}
{
}

QSize ExpansionState::sizeHint() const { return QSize(width, height); }


ExpandedFully::ExpandedFully(ExpandableWidget& widget)
    : ExpansionState{812, 500, widget}
{
}

void ExpandedFully::setStateUi()
{
    widget.ui->lvTaskView->setVisible(true);
    widget.ui->lvCompletedPomodoros->setVisible(true);
    widget.ui->pbAddTask->setVisible(true);
    widget.ui->leQuickAddTask->setVisible(true);
    widget.ui->pbAddPomodoroManually->setVisible(true);
    widget.ui->pbStatistics->setVisible(true);
    widget.ui->pbHistory->setVisible(true);
    widget.ui->pbGoals->setVisible(true);
    widget.ui->pbSettings->setVisible(true);
    widget.ui->pbToggleView->setText("Collapse");
    widget.ui->pbToggleMenu->setText("Hide menu");
}

void ExpandedFully::toggleView()
{
    widget.expansionState = widget.expandedMenuOnly.get();
}

void ExpandedFully::toggleMenu()
{
    widget.expansionState = widget.expandedWithoutMenu.get();
}


Shrinked::Shrinked(ExpandableWidget& widget)
    : ExpansionState{300, 450, widget}
{
}

void Shrinked::setStateUi()
{
    widget.ui->lvTaskView->setVisible(false);
    widget.ui->lvCompletedPomodoros->setVisible(false);
    widget.ui->pbAddTask->setVisible(false);
    widget.ui->leQuickAddTask->setVisible(false);
    widget.ui->pbAddPomodoroManually->setVisible(false);
    widget.ui->pbStatistics->setVisible(false);
    widget.ui->pbHistory->setVisible(false);
    widget.ui->pbGoals->setVisible(false);
    widget.ui->pbSettings->setVisible(false);
    widget.ui->pbToggleView->setText("Expand");
    widget.ui->pbToggleMenu->setText("Show menu");
}

void Shrinked::toggleView()
{
    widget.expansionState = widget.expandedWithoutMenu.get();
}

void Shrinked::toggleMenu()
{
    widget.expansionState = widget.expandedMenuOnly.get();
}


ExpandedMenuOnly::ExpandedMenuOnly(ExpandableWidget& widget)
    : ExpansionState{300, 500, widget}
{
}

void ExpandedMenuOnly::setStateUi()
{
    widget.ui->lvTaskView->setVisible(false);
    widget.ui->lvCompletedPomodoros->setVisible(false);
    widget.ui->pbAddTask->setVisible(false);
    widget.ui->leQuickAddTask->setVisible(false);
    widget.ui->pbAddPomodoroManually->setVisible(false);
    widget.ui->pbStatistics->setVisible(true);
    widget.ui->pbHistory->setVisible(true);
    widget.ui->pbGoals->setVisible(true);
    widget.ui->pbSettings->setVisible(true);
    widget.ui->pbToggleView->setText("Expand");
    widget.ui->pbToggleMenu->setText("Hide menu");
}

void ExpandedMenuOnly::toggleView()
{
    widget.expansionState = widget.expandedFully.get();
}

void ExpandedMenuOnly::toggleMenu()
{
    widget.expansionState = widget.shrinked.get();
}


ExpandedWithoutMenu::ExpandedWithoutMenu(ExpandableWidget& widget)
    : ExpansionState{812, 450, widget}
{
}

void ExpandedWithoutMenu::setStateUi()
{
    widget.ui->lvTaskView->setVisible(true);
    widget.ui->lvCompletedPomodoros->setVisible(true);
    widget.ui->pbAddTask->setVisible(true);
    widget.ui->leQuickAddTask->setVisible(true);
    widget.ui->pbAddPomodoroManually->setVisible(true);
    widget.ui->pbStatistics->setVisible(false);
    widget.ui->pbHistory->setVisible(false);
    widget.ui->pbGoals->setVisible(false);
    widget.ui->pbSettings->setVisible(false);
    widget.ui->pbToggleView->setText("Collapse");
    widget.ui->pbToggleMenu->setText("Show menu");
}

void ExpandedWithoutMenu::toggleView()
{
    widget.expansionState = widget.shrinked.get();
}

void ExpandedWithoutMenu::toggleMenu()
{
    widget.expansionState = widget.expandedFully.get();
}
