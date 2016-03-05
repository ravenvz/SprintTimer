#include "pomodoromodel.h"
#include "db_layer/db_service.h"
#include <QDebug>
#include <QSqlRecord>
#include <QDateTime>


PomodoroModel::PomodoroModel(QObject* parent) :
    SqliteTableModel(parent)
{
    setTable("pomodoro_view");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

QVariant PomodoroModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QSqlTableModel::data(index, role);
    }

    switch (role) {
        case Qt::DisplayRole:
            return itemAt(index.row()).toString();
        default:
            return QSqlTableModel::data(index, role);
    }
}

bool PomodoroModel::insert(const long long associatedTodoItemId, const TimeInterval& interval) {
    QSqlRecord rec = record();
    rec.setGenerated(static_cast<int>(Columns::Id), false);
    rec.setGenerated(static_cast<int>(Columns::Name), false);
    rec.setGenerated(static_cast<int>(Columns::Tags), false);
    rec.setValue(static_cast<int>(Columns::TodoId), associatedTodoItemId);
    rec.setValue(static_cast<int>(Columns::StartTime), interval.startTime);
    rec.setValue(static_cast<int>(Columns::FinishTime), interval.finishTime);
    return QSqlTableModel::insertRecord(-1, rec) && submitAll();
}

bool PomodoroModel::remove(const int row) {
    return removeRow(row) && submitAll();
}

Pomodoro PomodoroModel::itemAt(const int row) const {
    QSqlRecord rowRecord {QSqlTableModel::record(row)};
    QString name {columnData(rowRecord, Columns::Name).toString()};
    TimeInterval interval {columnData(rowRecord, Columns::StartTime).toDateTime(),
                           columnData(rowRecord, Columns::FinishTime).toDateTime()};
    QStringList tags {columnData(rowRecord, Columns::Tags).toString().split(",")};
    return Pomodoro {name, interval, tags};
}

QVector<Pomodoro> PomodoroModel::items() {
    auto numRows = numRecords();
    QVector<Pomodoro> result;
    result.reserve(numRows);
    for (int row = 0; row < numRows; ++row) {
        result.push_back(itemAt(row));
    }
    return result;
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

QStringList PomodoroModel::yearRange() {
    QStringList result;
    QSqlQuery yearRangeQuery;
    yearRangeQuery.exec("select distinct strftime('%Y', start_time) "
                        "from pomodoro order by start_time;");
    while (yearRangeQuery.next()) {
        result << yearRangeQuery.value(0).toString();
    }
    return result;
}

QVariant PomodoroModel::columnData(const QSqlRecord& rowRecord, const Columns& column) const {
    return rowRecord.value(static_cast<int>(column));
}

QVariant PomodoroModel::columnData(const QModelIndex& index, const Columns& column, int role) const {
    return data(index.model()->index(index.row(), static_cast<int>(column)), role);
}




