#ifndef DEVICELIST_H
#define DEVICELIST_H

#include <QDebug>

#include <QListView>
#include <QStandardItemModel>
#include <QUuid>

#include "actiondelegate.h"
#include "device.h"


class ActionList : public QListView
{
    Q_OBJECT
public:
    explicit ActionList(QWidget *parent = nullptr);

    void AddAction(const Action &, const QString &);
    void UpdateStatusAtIndex(const QModelIndex &index, const QString &status);

public slots:
    void AddActions(const Device &);
    void ClearAll();
};

#endif // DEVICELIST_H
