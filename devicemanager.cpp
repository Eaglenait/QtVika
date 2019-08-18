#include "devicemanager.h"

//TODO handle mobile lock on mdns lib (manually stop and start on screen lock/unlock
//call device action
//UI device List

DeviceManager::DeviceManager(QObject *parent)
    : QObject (parent)
    , manager(new QNetworkAccessManager(this))
    , zeroconf(new QZeroConf())
{
    //HTTP Handling
    QObject::connect(manager
                     , &QNetworkAccessManager::finished
                     , this
                     , &DeviceManager::HandleGetConfigResponse);

    //MDNS Handling
    zeroconf->startBrowser("_vika._tcp");
    QObject::connect(zeroconf
                     , &QZeroConf::serviceAdded
                     , this
                     , &DeviceManager::GetConfig);
}

DeviceManager::~DeviceManager(){
    delete manager;
    zeroconf->stopBrowser();
    delete zeroconf;
}

//Fix pls
void DeviceManager::isAlive() {
//    qDebug() << "isAlive";
//    QVector<int> toRemove;
//    QNetworkAccessManager *isAliveManager = new QNetworkAccessManager(this);
//
//    for(int i = 0; i < deviceList.length(); i++) {
//
//        QNetworkRequest req;
//
//        QString url = "http://" + deviceList[i].address.toString() + "/";
//        req.setUrl(QUrl(url));
//        req.setRawHeader("User-Agent", "Vika Volatile Server");
//
//        qDebug() << "isAlive - on url : " << url;
//        QNetworkReply *reply = isAliveManager->get(req);
//        if(reply->error()){
//            qDebug() << "isAlive error";
//            toRemove.push_back(i);
//        }
//    }
//
//    if(toRemove.length() != 0) {
//        foreach(const int idx, toRemove) {
//            toRemove.removeAt(idx);
//            qDebug() << "isAlive removed at idx " << idx;
//        }
//    }
//
}

void DeviceManager::CallAction(const Action &a) const {
    QHttpMultiPart *http = new QHttpMultiPart();
    QHttpPart part;
    QByteArray qb = "coucou";

    part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\""));
    part.setBody(qb);

    http->append(part);
    manager->post(a.req, http);

    //may be useless
    delete http;
}

void DeviceManager::HandleGetConfigResponse(QNetworkReply *reply) {
    qDebug() << "DeviceManager - Handling GetConfig";
    if(reply->error()){
        qDebug() << "  - HTTP reponse handling error: "
                 << reply->errorString();
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll()
                                                , &parseError);

    if(parseError.error != QJsonParseError::NoError) {
      qDebug() << "  - Error while parsing Http response";
      return;
    }

    if (!doc["a"].isArray())
        qDebug() << "- Device actions are not an array";
    else
        qDebug() << "  - Device actions are an array";


    //parse host addr from url
    QHostAddress replyHost = QHostAddress(QUrl(reply->url()).host());

    QVector<Action> actions;

    //Parse actions from json repsonse and build a device
    auto jsonActions = doc["a"].toArray();
    foreach(const QJsonValue & action, jsonActions){
        qDebug() << "  -Action found";

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

        ActionType actionType;
        QString actionTypeStr = doc["type"].toString();

        if(actionTypeStr == "Toggle") {
            actionType = ActionType::Toggle;
        } else if(actionTypeStr == "Analog") {
            actionType = ActionType::Analog;
        } else if(actionTypeStr == "Measure") {
            actionType = ActionType::Measure;
        } else {
            qDebug() << "DeviceManager - Undefined Action type, str: " << doc["type"].toString();
            actionType = ActionType::Undefined;
        }

        QString description	= doc["desc"].toString();
        QString uri = "http://" + replyHost.toString() + "/handlePin?a=" + actions.length();
        qDebug() << uri;

        actions.push_back(Action(actionType, syntax, uri, description));
        qDebug() << actions.last().description;
        actions.last().syntax.Print();
    }

    //Add device to device list
    deviceList.push_back(Device(replyHost, actions));

    emit DeviceDiscovered();
}

void DeviceManager::GetConfig(QZeroConfService service) {
    if(DeviceAddrIndexOf(service->ip()) != -1) {
        qDebug() << "DeviceManager - GetConfig abort : device already exists";
        return;
    }

    QString url = "http://" + service->ip().toString() + "/getConfig";

    qDebug() << "DeviceManager - Getting config on url " << url;

    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setRawHeader("User-Agent", "Vika Volatile Server");

    manager->get(req);
}

int DeviceManager::DeviceAddrIndexOf(const QHostAddress &addr) const {
    for (int i = 0; i < deviceList.length(); ++i) {
        if(deviceList[i].address == addr) {
            return i;
        }
    }

    return -1;
}
