#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "syntaxhelper.h"
#include "echoserver.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

#include <QtNetwork>
#include <QHostAddress>
#include <QNetworkAccessManager>
/*
    Device discovery
    Config parsing
    Device CRUD
*/

class VikaDevice {
public:
    QHostAddress DeviceAddress;
    QList<VikaSyntax> configs;

    VikaDevice();
};

class DeviceManager: QObject
{
    Q_OBJECT
public:
    EchoServer *httpServer;
    QNetworkAccessManager *manager;
    QList<VikaDevice> deviceList;

    explicit DeviceManager(QObject *parent = Q_NULLPTR);

    /// Sends a multicast request for every device to register to this server
    ///
    void AdvertiseServer();

    void DebugPrint() const;

    //get request to found modules on '/getConfig' url
    QVector<VikaSyntax> GetConfig(QHostAddress addr);

private:
    //Check if devices are alive
    //removes from deviceList is it isn't - returns number of devices removed
    //TODO run on timer
    int isAlive() const;

    QHostAddress multicastAddressv4;
    QHostAddress multicastAddressv6;
    QUdpSocket socketv4;
    QUdpSocket socketv6;
};

#endif // DEVICEMANAGER_H
