#ifndef POMODORO_POMODORO_H
#define POMODORO_POMODORO_H


#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
#include <QRegularExpression>
#include <experimental/optional>
#include "TodoItem.h"

class Pomodoro
{

public:
    Pomodoro();
    Pomodoro(const QString todoName, const QDateTime startTime, const QDateTime finishTime,
             const QStringList tags, long long assosiatedTodoItemId);
    Pomodoro(const TodoItem& todoItem, const QDateTime& startTime, const QDateTime& finishTime);

    const QString getName() const;
    void setName(const QString& name);
    const QDateTime getStartTime() const;
    void setStartTime(const QDateTime& startTime);
    const QDateTime getFinishTime() const;
    void setFinishTime(const QDateTime& finishTime);
    const QStringList getTags() const;
    const QString toString() const;

private:
    QString name;
    QDateTime startTime;
    QDateTime finishTime;
    QStringList tags;
    std::experimental::optional<long long> id;
    std::experimental::optional<long long> todoId;
    QString tagPrefix = "#";

};



#endif //POMODORO_POMODORO_H
