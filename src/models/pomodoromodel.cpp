#include "pomodoromodel.h"
#include "db_layer/db_service.h"
#include <QDateTime>
#include <QDebug>
#include <QSqlRecord>


PomodoroModel::PomodoroModel(QObject* parent)
    : SqliteTableModel(parent)
{
    setTable("pomodoro_view");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

QVariant PomodoroModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QSqlTableModel::data(index, role);
    }

    switch (role) {
    case Qt::DisplayRole:
        return QString::fromStdString(itemAt(index.row()).toString());
    default:
        return QSqlTableModel::data(index, role);
    }
}

bool PomodoroModel::insert(const long long associatedTodoItemId,
                           const TimeSpan& timeSpan)
{
    QSqlRecord rec = record();
    rec.setGenerated(static_cast<int>(Columns::Id), false);
    rec.setGenerated(static_cast<int>(Columns::Name), false);
    rec.setGenerated(static_cast<int>(Columns::Tags), false);
    rec.setValue(static_cast<int>(Columns::TodoId), associatedTodoItemId);
    rec.setValue(static_cast<int>(Columns::StartTime),
                 QDateTime::fromTime_t(
                     static_cast<unsigned>(timeSpan.startTime.toTime_t())));
    rec.setValue(static_cast<int>(Columns::FinishTime),
                 QDateTime::fromTime_t(
                     static_cast<unsigned>(timeSpan.finishTime.toTime_t())));
    return QSqlTableModel::insertRecord(-1, rec) && submitAll();
}

bool PomodoroModel::remove(const int row)
{
    return removeRow(row) && submitAll();
}

Pomodoro PomodoroModel::itemAt(const int row) const
{
    QSqlRecord rowRecord{QSqlTableModel::record(row)};
    QString name{columnData(rowRecord, Columns::Name).toString()};
    QDateTime start = columnData(rowRecord, Columns::StartTime).toDateTime();
    QDateTime finish = columnData(rowRecord, Columns::FinishTime).toDateTime();
    int offsetFromUtcInSeconds{start.offsetFromUtc()};
    TimeSpan timeSpan{
        start.toTime_t(), finish.toTime_t(), offsetFromUtcInSeconds};
    QStringList qTags{
        columnData(rowRecord, Columns::Tags).toString().split(",")};
    std::list<std::string> tags;
    std::transform(qTags.cbegin(),
                   qTags.cend(),
                   std::back_inserter(tags),
                   [](const auto& tag) { return tag.toStdString(); });

    return Pomodoro{name.toStdString(), timeSpan, tags};
}

std::vector<Pomodoro> PomodoroModel::items()
{
    auto numRows = numRecords();
    std::vector<Pomodoro> result;
    // result.reserve(numRows);
    for (int row = 0; row < numRows; ++row) {
        result.push_back(itemAt(row));
    }
    return result;
}

void PomodoroModel::setDateFilter(const DateInterval& timeSpan)
{
    QString filter{
        QString("date(start_time) >= '%1' and date(start_time) <= '%2'")
            .arg(timeSpan.startDate.toString("yyyy-MM-dd"))
            .arg(timeSpan.endDate.toString("yyyy-MM-dd"))};
    setFilter(filter);
}

void PomodoroModel::setSortByTime()
{
    setSort(static_cast<int>(Columns::StartTime), Qt::AscendingOrder);
}

QStringList PomodoroModel::yearRange()
{
    QStringList result;
    QSqlQuery yearRangeQuery;
    yearRangeQuery.exec("select distinct strftime('%Y', start_time) "
                        "from pomodoro order by start_time;");
    while (yearRangeQuery.next()) {
        result << yearRangeQuery.value(0).toString();
    }
    return result;
}

QVariant PomodoroModel::columnData(const QSqlRecord& rowRecord,
                                   const Columns& column) const
{
    return rowRecord.value(static_cast<int>(column));
}

QVariant PomodoroModel::columnData(const QModelIndex& index,
                                   const Columns& column,
                                   int role) const
{
    return data(index.model()->index(index.row(), static_cast<int>(column)),
                role);
}
