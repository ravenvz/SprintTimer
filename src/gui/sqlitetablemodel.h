#include <QSqlTableModel>

class SqliteTableModel : public QSqlTableModel
{
public:
    SqliteTableModel(QObject* parent = 0) :
        QSqlTableModel(parent)
    {

    }

    // As sqlite3 does not return query size, just calling rowCount() would return 
    // the number of rows that are currently cached on the client. 
    // This function first fetches all records, than return correct number of rows.
    int numRecords() {
        while (canFetchMore()) fetchMore();
        return QSqlTableModel::rowCount();  
    }

    // Wrapper around submitAll() method intended for using when there is a need
    // to submit a batch of cached changes.
    // QSqlTable model does not use transaction mechanism in submitAll method, 
    // therefore, wrapping it inside a transaction speeds up things singificantly
    // when dealing with multiple rows.
    bool submitAllInTransaction() {
        QSqlDatabase::database().transaction();
        return QSqlTableModel::submitAll() && QSqlDatabase::database().commit();
    }


};
