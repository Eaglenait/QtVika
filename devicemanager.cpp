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
    QObject::connect(manager, &QNetworkAccessManager::finished,
                     this, &DeviceManager::HandleGetConfigResponse);

    //MDNS Handling
    zeroconf->startBrowser("_vika._tcp");
    QObject::connect(zeroconf, &QZeroConf::serviceAdded,
                     this, &DeviceManager::GetConfig);
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
}

void DeviceManager::CallAction(const QModelIndex &index) const {
    QNetworkRequest req;
    QString url = index.data(Qt::UserRole + 2).toString();
    req.setUrl(QUrl(url));

    QHttpMultiPart *http = new QHttpMultiPart();
    QHttpPart part;
    QByteArray qb = "coucou";

    part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\""));
    part.setBody(qb);

    http->append(part);
    manager->post(req, http);

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

    QByteArray data = reply->readAll();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data
                                                , &parseError);

    if(parseError.error != QJsonParseError::NoError) {
      qDebug() << "  - Error while parsing Http response";
      return;
    }

    //parse host addr from url
    QHostAddress replyHost = QHostAddress(QUrl(reply->url()).host());

    QVector<Action> actions;

    //Parse actions from json repsonse and build a device
    QJsonArray jsonActions = doc["a"].toArray();
    for (int i = 0; i < jsonActions.size(); i++) {
        QJsonObject ac = jsonActions[i].toObject();
        VikaSyntax syntax;

        foreach(const QJsonValue & verb, ac["vrb"].toArray()){
            syntax.Verbs.append(verb.toString());
        }

        foreach(const QJsonValue & obj, ac["obj"].toArray()){
            syntax.Objects.append(obj.toString());
        }

        foreach(const QJsonValue & adj, ac["adj"].toArray()){
            syntax.Adjectives.append(adj.toString());
        }

        syntax.Localisation = ac["loc"].toString();

        QString description	= ac["desc"].toString();
        QString uri = "http://" + replyHost.toString() + "/handlePin?a=" + i;

        ActionType actionType;
        QString actionTypeStr = ac["type"].toString();

        if(actionTypeStr == "Toggle") {
            actionType = ActionType::Toggle;
        } else if(actionTypeStr == "Analog") {
            actionType = ActionType::Analog;
        } else if(actionTypeStr == "Measure") {
            actionType = ActionType::Measure;
        } else {
            qDebug() << "DeviceManager - Undefined Action type, str: " << ac["type"].toString();
            actionType = ActionType::Undefined;
        }

        actions.push_back(Action(actionType, syntax, uri, description));
    }

    Device d = Device(replyHost, actions);
    //Add device to device list
    deviceList.push_back(d);

    emit DeviceDiscovered(d);
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
