#include "pomodoroquerymodel.h"
#include "db_layer/db_helper.h"
#include <QDebug>
#include <QSqlRecord>
#include <QDateTime>

PomodoroQueryModel::PomodoroQueryModel(QObject* parent) :
    QSqlQueryModel(parent) 
{

}

QVariant PomodoroQueryModel::data(const QModelIndex& index, int role) const {
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

Pomodoro PomodoroQueryModel::rowToPomodoro(const QModelIndex& index, int role) const {
    QString name = columnData(index, Columns::Name).toString();
    TimeInterval interval {columnData(index, Columns::StartTime).toDateTime(),
                           columnData(index, Columns::FinishTime).toDateTime()};
    QStringList tags = columnData(index, Columns::Tags).toString().split(",");
    long long id_to_remove = columnData(index, Columns::TodoId).toInt();

    return Pomodoro {name, interval, tags, id_to_remove};
}

QVariant PomodoroQueryModel::columnData(const QModelIndex& index, const Columns& column, int role) const {
    return QSqlQueryModel::data(index.model()->index(index.row(), static_cast<int>(column)), role);
}

void PomodoroQueryModel::removePomodoro(const QModelIndex& index) {
    QSqlQuery removePomodoroQuery = PomodoroDataSource::buildQueryToRemovePomodoro(
            columnData(index, Columns::Id).toInt()
        );
    removePomodoroQuery.exec();
    refresh();
}

void PomodoroQueryModel::refresh() {
    // clear();
    QSqlQueryModel::setQuery(query().executedQuery());
}
