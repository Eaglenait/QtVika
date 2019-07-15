#include "devicemanager.h"

VikaDevice::VikaDevice() {}

DeviceManager::DeviceManager(QObject *parent)
    : QObject (parent)
    , manager(new QNetworkAccessManager(this))
    , multicastAddressv4(QStringLiteral("224.0.0.251"))
{
    bool bindResult = UDPsocket.bind(QHostAddress(QHostAddress::Any), 0);
    if(!bindResult)
        qDebug() << "Multicast bind error";
    else
        qDebug() << "Multicast bind success";

    connect(qMDNS::getInstance()
            , &qMDNS::hostFound
            , this
            , &DeviceManager::HostFound);
}

DeviceManager::~DeviceManager() {
}

void DeviceManager::FindService(const QString& service) {
    qDebug() << "finding service :" << service;
    qMDNS::getInstance()->lookup(service);
}

void DeviceManager::HostFound(const QHostInfo& info) const {
    qDebug() << "mdns host found on addr: " << info.addresses().first();
}

QVector<VikaSyntax> DeviceManager::GetConfig(QHostAddress addr) {
    //TODO: get request to addr
    QNetworkRequest req;
    QString url = "http://" + addr.toString() + "/getConfig";
    req.setUrl(QUrl(url));
    req.setRawHeader("User-Agent", "Vika Volatile Server");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");

    QVector<VikaSyntax> syntaxes;

    manager->get(req);

    //fix requests

    QByteArray replyMessage;
    QObject::connect(manager, &QNetworkAccessManager::finished, this,
                     [=, &replyMessage](QNetworkReply *reply) {
      if(reply->error()) {
        qDebug() << reply->errorString();
        return syntaxes;
      }

      replyMessage = reply->readAll();
      qDebug() << "answer : " << replyMessage;
    });

    qDebug() << "getConfig response : " << replyMessage;

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(replyMessage, &parseError);

    if(parseError.error != QJsonParseError::NoError) {
      qDebug() << "reply is not json";
      return syntaxes;
    }

    if (!doc["a"].isArray()) qDebug() << "response is not array";
    else qDebug() << "response is array";

    auto jsonActions = doc["a"].toArray();

    foreach(const QJsonValue & action, jsonActions){
      VikaSyntax syntax;

      foreach(const QJsonValue & verb, action["vrb"].toArray()){
        syntax.Verbs.append(verb.toString());
      }

      foreach(const QJsonValue & obj, action["obj"].toArray()){
        syntax.Objects.append(obj.toString());
      }

      foreach(const QJsonValue & adj, action["adj"].toArray()){
        syntax.Adjectives.append(adj.toString());
      }

      syntax.Localisation = doc["loc"].toString();
      syntax.description 	= doc["desc"].toString();

      syntaxes.append(syntax);
    }

    //Default: return empty syntax
    return syntaxes;
}

void DeviceManager::AdvertiseServer() {
  qDebug() << "Advertising server";
  QByteArray datagram = "Vika Server Advertisement";

  UDPsocket.writeDatagram(datagram, multicastAddressv4, 45454);

}

int DeviceManager::isAlive() const {

    //foreach(auto & device, deviceList) {
        //TODO : ping device, remove from devicelist if no response
    //}

    //TODO: Return number of devices removed ? (or device currently alive)
    return 0;
}
