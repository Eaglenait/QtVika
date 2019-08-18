#ifndef DEVICELIST_H
#define DEVICELIST_H

#include <QListView>
#include <QStandardItemModel>

#include "devicedelegate.h"
#include "device.h"

class DeviceList : public QListView
{
    Q_OBJECT
public:
    explicit DeviceList(QWidget *parent = nullptr);

public slots:
    void AddDevice(const QString &description, const QString &keyword, const QString &state);
    void ClearAll();
};

#endif // DEVICELIST_H
