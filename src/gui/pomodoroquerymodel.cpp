#include "pomodoroquerymodel.h"
#include "db_layer/db_helper.h"
#include <QDebug>
#include <QSqlRecord>
#include <QDateTime>


PomodoroModel::PomodoroModel(QObject* parent) :
    QSqlTableModel(parent) 
{

}

QVariant PomodoroModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
        case Qt::DisplayRole:
            return rowToPomodoro(index).toString();
        default:
            return QVariant();
    }
}

// bool PomodoroModel::setData(const QModelIndex& index, const QVariant& value, int role) {
//     if (role == Qt::EditRole) {
//         qDebug() << "Edit role";
//     }
//     if (!index.isValid()) {
//         return false;
//     } 
//
//     if (role == Qt::EditRole) {
//         qDebug() << "Removing row. Sort of...";
//         QSqlTableModel::removeRows(index.row(), 1);
//         emit dataChanged();
//     }
//     
//     return false;
// }

Pomodoro PomodoroModel::rowToPomodoro(const QModelIndex& index, int role) const {
    QString name = columnData(index, Columns::Name).toString();
    TimeInterval interval {columnData(index, Columns::StartTime).toDateTime(),
                           columnData(index, Columns::FinishTime).toDateTime()};
    QStringList tags = columnData(index, Columns::Tags).toString().split(",");
    long long id_to_remove = columnData(index, Columns::TodoId).toInt();

   return Pomodoro {name, interval, tags, id_to_remove};
}

QVariant PomodoroModel::columnData(const QModelIndex& index, const Columns& column, int role) const {
    return QSqlQueryModel::data(index.model()->index(index.row(), static_cast<int>(column)), role);
}

void PomodoroModel::removePomodoro(const QModelIndex& index) {
    // TODO handle sad path
    removePomodoroFunctor(columnData(index, Columns::Id));
    select();
}

void PomodoroModel::insertPomodoro(const Pomodoro& pomodoro, long long associatedTodoItemId) {
    // TODO handle sad path
    insertPomodoroFunctor(pomodoro, associatedTodoItemId);
    select();
}

void PomodoroModel::setRemovePomodoroFunctor(std::function<void(QVariant)> func) {
    removePomodoroFunctor = func;
}

void PomodoroModel::setInsertPomodoroFunctor(std::function<bool (const Pomodoro& pomodoro, long long associatedTodoItemId)> func) {
    insertPomodoroFunctor = func;
}
