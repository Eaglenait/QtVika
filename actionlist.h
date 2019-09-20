#ifndef DEVICELIST_H
#define DEVICELIST_H

#include <QDebug>

#include <QListView>
#include <QStandardItemModel>

#include "actiondelegate.h"
#include "device.h"

class ActionList : public QListView
{
    Q_OBJECT
public:
    explicit ActionList(QWidget *parent = nullptr);

public slots:
    void UpdateStatusAtIndex(const QModelIndex &index, const QString &status);
    void AddActions(const Device &);
    void ClearAll();

private:
    void AddAction(const Action &, const QString &);
};

#endif // DEVICELIST_H
