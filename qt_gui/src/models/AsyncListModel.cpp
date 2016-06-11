#include "models/AsyncListModel.h"
#include <iostream>

AsyncListModel::AsyncListModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

void AsyncListModel::silentUpdate()
{
    silent = true;
    requestDataUpdate();
}

void AsyncListModel::broadcastUpdateFinished()
{
    if (silent) {
        silent = false;
        return;
    }
    emit updateFinished();
}
