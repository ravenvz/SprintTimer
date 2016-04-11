#ifndef POMODOROMODEL_H
#define POMODOROMODEL_H

#include "core/entities/Pomodoro.h"
#include "sqlitetablemodel.h"
#include "utils/DateInterval.h"
#include <QSqlQuery>

class PomodoroModel : public SqliteTableModel {
public:
    explicit PomodoroModel(QObject* parent = 0);

    // Override to supply custom DisplayRole behaviour.
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;

    // Insert pomodoro identified by time timeSpan and it's associated TodoItem.
    // Return boolean, indicating success of the operation.
    // Note that method doesn't check if associated TodoItem with given id
    // actually
    // exists in the database.
    bool insert(const long long associatedTodoItemId, const TimeSpan& timeSpan);

    // Remove pomodoro at given row number.
    // Return boolean, indicating success of the operation.
    bool remove(const int row);

    // Return pomodoro at given row.
    Pomodoro itemAt(const int row) const;

    // Return vector, containing all pomodoros.
    std::vector<Pomodoro> items();

    // Only pomodoros that have starting time in the given timeSpan will be
    // in the model.
    void setDateFilter(const DateInterval& timeSpan);

    // Pomodoros in the model will be sorted by starting time ascending.
    void setSortByTime();

    // Return list containing records' years in ascending order.
    QStringList yearRange();

private:
    enum class Columns {
        Id = 0,
        TodoId,
        Name,
        Tags,
        StartTime,
        FinishTime,
    };

    // Return value at given index and column wrapped in QVariant.
    QVariant columnData(const QModelIndex& index,
                        const Columns& column,
                        int role = Qt::DisplayRole) const;

    // Overloaded method.
    // Return value at given record and column wrapped in QVarian.
    QVariant columnData(const QSqlRecord& rowRecord,
                        const Columns& column) const;
};


#endif /* end of include guard: POMODOROMODEL_H */
