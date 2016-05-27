#include "tagmodel.h"


TagModel::TagModel(QObject* parent) :
    QSqlTableModel(parent)
{
    setTable("tag");
    setSort(static_cast<int>(Columns::Name), Qt::AscendingOrder);
    setEditStrategy(QSqlTableModel::OnFieldChange);
}
