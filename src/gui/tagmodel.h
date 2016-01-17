#ifndef TAGMODEL_H
#define TAGMODEL_H


#include <QSqlTableModel>


class TagModel : public QSqlTableModel 
{

public:
    explicit TagModel(QObject* parent = 0);


private:
    enum class Columns {
        Id,
        Name,
    };

};


#endif /* end of include guard: TAGMODEL_H */
