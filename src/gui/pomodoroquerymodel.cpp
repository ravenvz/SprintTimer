#include "pomodoroquerymodel.h"
#include "db_layer/db_helper.h"
#include <QDebug>
#include <QSqlRecord>
#include <QDateTime>


PomodoroModel::PomodoroModel(QObject* parent) :
    QSqlTableModel(parent) 
{
    setTable("pomodoro_view");
    removePomodoroQuery.prepare("delete from pomodoro where id = (:id)");
    insertPomodoroQuery.prepare("insert into pomodoro (name, start_time, finish_time, todo_id)" 
                                "values (:name, :start_time, :finish_time, :todo_id)");
}

QVariant PomodoroModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QSqlTableModel::data(index, role);
    }

    switch (role) {
        case Qt::DisplayRole:
            return rowToPomodoro(index.row()).toString();
        case GetPomodoroIdRole:
            return columnData(index, Columns::Id);
        default:
            return QSqlTableModel::data(index, role);
    }
}

QVector<Pomodoro> PomodoroModel::pomodoros() {
    while (canFetchMore()) fetchMore();
    QVector<Pomodoro> result;
    result.reserve(rowCount());
    for (int i = 0; i < QSqlTableModel::rowCount(); ++i) {
        result.push_back(rowToPomodoro(i));
    }
    return result;
}

Pomodoro PomodoroModel::rowToPomodoro(const int row) const {
    QSqlRecord rowRecord {QSqlTableModel::record(row)};
    QString name {columnData(rowRecord, Columns::Name).toString()};
    TimeInterval interval {columnData(rowRecord, Columns::StartTime).toDateTime(),
                           columnData(rowRecord, Columns::FinishTime).toDateTime()};
    QStringList tags {columnData(rowRecord, Columns::Tags).toString().split(",")};
    long long id_to_remove = columnData(rowRecord, Columns::TodoId).toInt();
    return Pomodoro {name, interval, tags, id_to_remove};
}

QVariant PomodoroModel::columnData(const QSqlRecord& rowRecord, const Columns& column) const {
    return rowRecord.value(static_cast<int>(column));
}

QVariant PomodoroModel::columnData(const QModelIndex& index, const Columns& column, int role) const {
    return data(index.model()->index(index.row(), static_cast<int>(column)), role);
}

void PomodoroModel::removePomodoro(const QModelIndex& index) {
    // TODO handle sad path
    QVariant id {columnData(QSqlTableModel::record(index.row()), Columns::Id)};
    removePomodoroQuery.bindValue(":id", id);
    removePomodoroQuery.exec();
    select();
}

void PomodoroModel::insertPomodoro(const Pomodoro& pomodoro, long long associatedTodoItemId) {
    // TODO handle sad path
    insertPomodoroQuery.bindValue(":name", QVariant(pomodoro.getName()));
    insertPomodoroQuery.bindValue(":start_time", QVariant(pomodoro.getStartTime()));
    insertPomodoroQuery.bindValue(":finish_time", QVariant(pomodoro.getFinishTime()));
    insertPomodoroQuery.bindValue(":todo_id", QVariant(associatedTodoItemId));
    insertPomodoroQuery.exec();
    select();
}

void PomodoroModel::setDateFilter(const DateInterval& interval) {
    QString filter {QString("date(start_time) >= '%1' and date(start_time) <= '%2'")
        .arg(interval.startDate.toString("yyyy-MM-dd"))
        .arg(interval.endDate.toString("yyyy-MM-dd"))};
    setFilter(filter);
}

void PomodoroModel::setSortByTime() {
    setSort(static_cast<int>(Columns::StartTime), Qt::AscendingOrder);
}

// TODO a bit off here, at it does not belong to this model
QStringList PomodoroModel::yearRange() {
    // TODO handle sad path
    QStringList result;
    QSqlQuery yearRangeQuery;
    yearRangeQuery.exec("select distinct strftime('%Y', start_time) "
                        "from pomodoro order by start_time;");
    while (yearRangeQuery.next()) {
        result << yearRangeQuery.value(0).toString();
    }
    return result;
}
