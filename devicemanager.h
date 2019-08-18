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
    /// \brief Parses the response and adds the device to the devicelist
    /// - emits : DeviceManager::DeviceAdded
    void HandleGetConfigResponse(QNetworkReply *);

    /// \brief Gets device config on zeroconf discover
    void GetConfig(QZeroConfService);

    /// \brief Goes through the list of devices and removes those who doesn't respond
    /// - emits : DeviceMissing
    void isAlive();

    /// \brief Calling an action
    void CallAction(const Action &a) const;

signals:
    void DeviceDiscovered() const;

    /// \brief Emitted on failure to respond to IsAlive request
    /// \param Addresses of non-responding devices
    void DeviceMissing(QList<QHostAddress>) const;

private:
    QZeroConf *zeroconf;
    QTimer *t;

    /// \brief Returns the index of the device with the given IP address
    int DeviceAddrIndexOf(const QHostAddress &) const;
};

#endif // DEVICEMANAGER_H
