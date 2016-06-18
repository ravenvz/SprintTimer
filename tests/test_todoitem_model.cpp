// #include "db_layer/db_service.h"
#include "models/TaskModel.h"
#include <TestHarness.h>


TEST_GROUP(TaskModel)
{
    const int defaultEstimatedPomos{1};
    const int defaultSpentPomos{0};
    enum Column {
        TodoId,
        TodoName,
        EstimatedPomodoros,
        SpentPomodoros,
        Priority,
        Completed,
        Tags,
        LastModified
    };

    void setup()
    {
        // QSqlTableModel confuses memory leak checker
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
        QSqlQuery query;
        query.exec("delete from todo_item;");
        query.exec("delete from todotag;");
        query.exec("delete from tag;");
    }

    void teardown() { MemoryLeakWarningPlugin::turnOnNewDeleteOverloads(); }

    bool todo_item_equal(const TodoItem& item1, const TodoItem& item2)
    {
        if (item1.name() != item2.name()
            || item1.estimatedPomodoros() != item2.estimatedPomodoros()
            || item1.spentPomodoros() != item2.spentPomodoros()
            || item1.isCompleted() != item2.isCompleted()) {
            return false;
        }
        // Tags are compared sorted, because there is no guarantee of tag
        // ordering
        std::list<std::string> tags1 = item1.tags();
        std::list<std::string> tags2 = item2.tags();
        tags1.sort();
        tags2.sort();
        return tags1 == tags2;
    }
};

TEST(TaskModel, test_relation_created_when_inserting_with_few_new_tags)
{
    TaskModel model;
    model.select();
    std::string name{"Test item"};
    std::list<std::string> tags{"Tag1", "Tag2"};
    TodoItem item{name, defaultEstimatedPomos, defaultSpentPomos, tags, false};

    CHECK(model.insert(item));
    CHECK(todo_item_equal(item, model.itemAt(0)));
}

TEST(TaskModel, test_insertion_with_no_tags)
{
    TaskModel model;
    TodoItem item{
        "Test item", defaultEstimatedPomos, defaultSpentPomos, {}, false};

    CHECK(model.insert(item));
    CHECK(todo_item_equal(item, model.itemAt(0)));
}

TEST(TaskModel, test_insertion_with_mixed_old_and_new_tags)
{
    TaskModel model;
    TodoItem testItem{"Test item",
                      defaultEstimatedPomos,
                      defaultSpentPomos,
                      {"Tag2", "Tag3"},
                      false};

    CHECK(model.insert(TodoItem{"Item 1",
                                defaultEstimatedPomos,
                                defaultSpentPomos,
                                {"Tag1", "Tag2"},
                                false}));
    CHECK(model.insert(testItem));
    CHECK(todo_item_equal(testItem, model.itemAt(1)));
}

TEST(TaskModel, test_remove_todo_item_with_no_related_tags)
{
    TaskModel model;
    CHECK(model.insert(TodoItem{
        "Test item", defaultEstimatedPomos, defaultSpentPomos, {}, false}));
    CHECK(model.numRecords() == 1);
    CHECK(model.remove(0));
    CHECK(model.numRecords() == 0);
}

TEST(TaskModel, test_remove_todo_item_should_clean_orphaned_tags)
{
    TaskModel model;

    CHECK(model.insert(TodoItem{"Test item",
                                defaultEstimatedPomos,
                                defaultSpentPomos,
                                {"Tag1, Tag2"},
                                false}));
    CHECK(model.remove(0));

    CHECK_EQUAL(0, model.numRecords());

    // Check todotag relation removed
    QSqlQuery query;
    query.exec("select count(*) from todotag;");
    CHECK(query.next());
    CHECK_EQUAL(0, query.value(0).toInt());

    // Check tags are removed
    query.exec("select count(*) from tag");
    CHECK(query.next());
    CHECK_EQUAL(0, query.value(0).toInt());
}

TEST(TaskModel,
     test_remove_todo_item_should_not_remove_tags_if_not_orphaned)
{
    TaskModel model;

    CHECK(model.insert(TodoItem{
        "Item 1", defaultEstimatedPomos, defaultSpentPomos, {"Tag1"}, false}));
    CHECK(model.insert(TodoItem{
        "Item 2", defaultEstimatedPomos, defaultSpentPomos, {"Tag1"}, false}));
    CHECK(model.remove(0));

    // Check only one relation left
    QSqlQuery query;
    query.exec("select count(*) from todotag;");
    CHECK(query.next());
    CHECK_EQUAL(1, query.value(0).toInt());

    // Check tag is not removed
    query.exec("select count(*) from tag");
    CHECK(query.next());
    CHECK_EQUAL(1, query.value(0).toInt());
}

TEST(TaskModel, test_toggle_item_completed)
{
    TaskModel model;
    TodoItem testItem{"Test item",
                      defaultEstimatedPomos,
                      defaultSpentPomos,
                      {"Tag2", "Tag3"},
                      false};
    CHECK(model.insert(testItem));

    model.toggleCompleted(model.index(0, 1));
    CHECK(model.data(model.index(0, 1), Qt::CheckStateRole).toBool());
    model.toggleCompleted(model.index(0, 1));
    CHECK(!model.data(model.index(0, 1), Qt::CheckStateRole).toBool());
}

TEST(TaskModel, test_get_all_items)
{
    TaskModel model;
    int numRecordsToAdd{300};
    TodoItem item{
        "Whatever", defaultEstimatedPomos, defaultSpentPomos, {}, false};
    for (int i = 0; i < numRecordsToAdd; ++i) {
        model.insert(item);
    }
    CHECK_EQUAL(numRecordsToAdd, model.numRecords());
    std::vector<TodoItem> allItems = model.items();
    CHECK_EQUAL(static_cast<size_t>(numRecordsToAdd), allItems.size());
}

TEST(TaskModel, test_update_item_with_no_tag_updating)
{
    TaskModel model;
    TodoItem item{"Initial name",
                  defaultEstimatedPomos,
                  defaultSpentPomos,
                  {"Tag1", "Tag2"},
                  false};
    TodoItem updatedItem{"New name after update", 4, 2, {"Tag1", "Tag2"}, true};

    CHECK(model.insert(item));

    CHECK(model.replaceItemAt(0, updatedItem));
    CHECK(todo_item_equal(updatedItem, model.itemAt(0)));
}

TEST(TaskModel, test_update_item_with_tags)
{
    TaskModel model;
    TodoItem item{"Initial name",
                  defaultEstimatedPomos,
                  defaultSpentPomos,
                  {"Tag1", "Tag2", "Tag3"},
                  false};
    TodoItem updatedItem{"New name after update", 4, 2, {"Tag3", "Tag4"}, true};

    CHECK(model.insert(item));

    CHECK(model.replaceItemAt(0, updatedItem));
    CHECK(todo_item_equal(updatedItem, model.itemAt(0)));

    QSqlQuery query;
    // Check no longer used tags are removed

    CHECK(query.exec("select count(*) from tag where name in ('Tag1', 'Tag2')")
          && query.next());
    CHECK_EQUAL(0, query.value(0).toInt());

    // Check new tags are inserted
    CHECK(query.exec("select count(*) from tag where name in ('Tag3', 'Tag4')")
          && query.next());
    CHECK_EQUAL(2, query.value(0).toInt());
}
