#ifndef POMODOROQUERYMODEL_H
#define POMODOROQUERYMODEL_H

#include <QSqlTableModel>
#include <QSqlQuery>
#include <functional>
#include "core/entities/Pomodoro.h"

class PomodoroModel : public QSqlTableModel
{
public:
    explicit PomodoroModel(QObject* parent = 0);
    QVector<Pomodoro> pomodoros();
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    // bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    void removePomodoro(const QModelIndex& index);
    void insertPomodoro(const Pomodoro& pomodoro, long long associatedTodoItemId);

    void setDateFilter(const DateInterval& interval);
    void setSortByTime();

    QStringList yearRange();

    enum class Columns {
        Id = 0,
        TodoId,
        Name,
        Tags,
        StartTime,
        FinishTime,
    };
    enum customRoles { GetPomodoroIdRole = Qt::UserRole + 1 };


private:
    QSqlQuery removePomodoroQuery;
    QSqlQuery insertPomodoroQuery;

    Pomodoro rowToPomodoro(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant columnData(const QModelIndex& index, const Columns& column, int role = Qt::DisplayRole) const;
    Pomodoro rowToPomodoro(const int row) const;
    QVariant columnData(const QSqlRecord& rowRecord, const Columns& column) const;

};



#endif /* end of include guard: POMODOROQUERYMODEL_H */
