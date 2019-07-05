#include "devicemanager.h"

VikaDevice::VikaDevice() {}

DeviceManager::DeviceManager(QObject *parent)
    : QObject (parent)
    , manager(new QNetworkAccessManager(this))
    , multicastAddressv4(QStringLiteral("224.0.0.251"))
    , multicastAddressv6(QStringLiteral("ff12::2115"))
{
    bool bindv4Result = socketv4.bind(QHostAddress(QHostAddress::AnyIPv4), 0);
    bool bindv6Result = socketv6.bind(QHostAddress(QHostAddress::AnyIPv6), socketv4.localPort());
    if(!bindv4Result)
        qDebug() << "Multicast ipv4 bind error";
    else
        qDebug() << "Multicast bind ipv4 success";

    if(!bindv6Result)
        qDebug() << "Multicast ipv6 bind error";
    else
        qDebug() << "Multicast bind ipv6 success";

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
    }
    );

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

  socketv4.writeDatagram(datagram, multicastAddressv4, 45454);
  if(socketv6.state() == QAbstractSocket::BoundState)
    socketv6.writeDatagram(datagram, multicastAddressv6, 45454);
}

int DeviceManager::isAlive() const {

    //foreach(auto & device, deviceList) {
        //TODO : ping device, remove from devicelist if no response
    //}

    //TODO: Return number of devices removed ? (or device currently alive)
    return 0;
}

void DeviceManager::DebugPrint() const {
}
