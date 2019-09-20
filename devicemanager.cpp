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

    QObject::connect(zeroconf, &QZeroConf::serviceRemoved,
                     this, &DeviceManager::RemoveDevice);

    //IsAlive Timer connect
}

DeviceManager::~DeviceManager(){
    delete manager;

    zeroconf->stopBrowser();
    delete zeroconf;
}

void DeviceManager::RemoveDevice(QZeroConfService &service)
{
    for (int i = 0; i < deviceList.size(); ++i) {
        if (deviceList[i].address == service->ip()) {
            deviceList.removeAt(i);
            emit DeviceMissing();
        }
    }
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
    QNetworkAccessManager *mngr = new QNetworkAccessManager();
    QEventLoop loop;
    QObject::connect(mngr, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    //get action url
    QNetworkRequest req;
    req.setUrl(QUrl(index.data(Qt::UserRole + 2).toString()));

    qDebug() << "calling action on row " << index.row();
    qDebug() << "    with url " << req.url();

    QByteArray qb = "";
    req.setHeader(QNetworkRequest::ContentDispositionHeader,
                   QVariant("form-data; name=\"data\""));

    req.setHeader(QNetworkRequest::ContentTypeHeader,
                  QVariant("application/json"));

    QNetworkReply *postReply = manager->post(req, qb);
    loop.exec();

    qDebug() << postReply->readAll();

    QString state = "";

    while(postReply->isFinished() == false) {
       qDebug() << ".";
    }

    if(postReply->error()) {
        qDebug() << "http post reply error";
    }

    qDebug() << "callaction response : state: " << state;

    emit ActionCalled(index, state);

    //delete http;
    delete postReply;
    delete mngr;
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
