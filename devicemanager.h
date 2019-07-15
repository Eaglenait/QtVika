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
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"

#include <qMDNS.h>

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

class DeviceManager: public QObject {
    Q_OBJECT
public:
    EchoServer *httpServer;
    QNetworkAccessManager *manager;
    QList<VikaDevice> deviceList;

    QWebSocket *socket;

    explicit DeviceManager(QObject *parent = Q_NULLPTR);
    ~DeviceManager();

    /// Sends a multicast request for every device to register to this server
    ///
    void AdvertiseServer();

    //get request to found modules on '/getConfig' url
    QVector<VikaSyntax> GetConfig(QHostAddress addr);
    void HostFound(const QHostInfo& addr) const;
    void FindService(const QString &service);


private:
    //Check if devices are alive
    //removes from deviceList is it isn't - returns number of devices removed
    //TODO background run on timer
    int isAlive() const;

    QHostAddress multicastAddressv4;
    QUdpSocket UDPsocket;
};

#endif // DEVICEMANAGER_H
