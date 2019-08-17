#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , deviceManager(new DeviceManager())
  , syntaxHelper(new SyntaxHelper())
  , mngr(new QNetworkAccessManager())
{
    ui->setupUi(this);

    //On new device add to UI list
    QObject::connect(deviceManager, &DeviceManager::DeviceAdded, this, &MainWindow::AddDevice);
    QObject::connect(deviceManager, &DeviceManager::DeviceRemoved, this, &MainWindow::RemoveDevice);
    QObject::connect(mngr, &QNetworkAccessManager::finished, this, &MainWindow::HandleResponse);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete deviceManager;
}

void MainWindow::AddDevice() const {
    qDebug() << "add device";
    ui->lst_devices->addItem(deviceManager->deviceList.last().address.toString());
}

void MainWindow::RemoveDevice(const int index) const {
    ui->lst_devices->takeItem(index);
}

void MainWindow::on_lst_devices_clicked(const QModelIndex &index) const
{
    QNetworkRequest req = deviceManager->deviceList.at(index.row()).actions.first().req;

    //FIX pour avoir la bonne action
    // probablement avec le nouvel UI

    QHttpMultiPart *http = new QHttpMultiPart();
    QHttpPart part;
    QByteArray qb = "coucou";

    part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\""));
    part.setBody(qb);

    http->append(part);
    mngr->post(req, http);
}

void MainWindow::HandleResponse(QNetworkReply *reply){
    if(reply->error()) {
       qDebug() << reply->errorString();
    }

    qDebug() << "handling call response" << reply->readAll();
}
