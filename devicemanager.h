#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "syntaxhelper.h"

#include <QTimer>
#include "qzeroconf.h"
#include "qzeroconfservice.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

#include <QtNetwork>
#include <QHostAddress>
#include <QNetworkAccessManager>

#include "device.h"

class DeviceManager: public QObject {
    Q_OBJECT
public:
    QNetworkAccessManager *manager;
    QList<Device> deviceList;

    explicit DeviceManager(QObject *parent = Q_NULLPTR);
    ~DeviceManager();

public slots:
    void HandleHttpReponse(QNetworkReply *);
    void GetConfig(QZeroConfService);
    void RemoveDevice(QZeroConfService);

signals:
    void DeviceAdded() const;
    void DeviceRemoved(const int index) const ;

private:
    QZeroConf *zeroconf;
    QTimer *t;

    int DeviceAddrIndexOf(const QHostAddress &) const;
};

#endif // DEVICEMANAGER_H
