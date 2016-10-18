#include "TestAddTask.h"
#include <QSpinBox>
#include <QtWidgets/QDialogButtonBox>

namespace {

void simulateWidgetExpand(MainWindow* mainWindow)
{
    auto expandButton = mainWindow->findChild<QPushButton*>("pbToggleView");
    auto addTaskButton = mainWindow->findChild<QPushButton*>("pbAddTask");
    QTest::mouseClick(expandButton, Qt::MouseButton::LeftButton);
    QTest::qWait(500);
    QTest::mouseClick(addTaskButton, Qt::MouseButton::LeftButton);
    QTest::qWait(500);
}

void simulateAddTask(QWidget* dialog,
                     const QString& name,
                     int cost,
                     const QString& tags)
{
    auto taskName = dialog->findChild<QLineEdit*>("taskName");
    taskName->setText(name);
    auto estimatedCost = dialog->findChild<QSpinBox*>("estimatedCost");
    estimatedCost->setValue(cost);
    auto tagsInput = dialog->findChild<QLineEdit*>("leTags");
    tagsInput->setText(tags);
    auto okButton = dialog->findChild<QDialogButtonBox*>("buttonBox");
    QTest::qWait(1000);
    okButton->accepted();
}

bool taskOutlineContains(QWidget* taskOutline, const QString& taskDescription)
{
    auto taskList = taskOutline->findChild<ReordableListView*>("lvTaskView");
    auto model = taskList->model();

    for (int i = 0; i < model->rowCount(); ++i) {
        if (taskDescription == model->data(model->index(i, 0)))
            return true;
    }

    return false;
}

} // namespace

TestAddTask::TestAddTask(QApplication* app, MainWindow* mainWindow)
    : app{app}
    , mainWindow{mainWindow}
{
}

void TestAddTask::cleanupTestCase()
{
    if (QFile::exists("sprint_test.db"))
        QFile::remove("sprint_test.db");
}

void TestAddTask::add_task()
{
    simulateWidgetExpand(mainWindow);

    if (auto dialog = app->activeWindow()) {
        simulateAddTask(dialog, "Test task", 4, "Tag1 Tag2");
    }
    auto taskOutline = mainWindow->findChild<QWidget*>("TaskOutline");
    QTest::qWait(500);

    QVERIFY(taskOutlineContains(taskOutline, QString{"#Tag1 #Tag2 Test task 0/4"}));
}
