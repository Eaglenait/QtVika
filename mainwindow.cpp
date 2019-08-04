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

    ui->MainLayout->setOriginCorner(Qt::Corner::TopLeftCorner);

    //On new device add to UI list
    QObject::connect(deviceManager, &DeviceManager::DeviceAdded, this, &MainWindow::AddDevice);
    QObject::connect(deviceManager, &DeviceManager::DeviceRemoved, this, &MainWindow::RemoveDevice);
    QObject::connect(mngr, &QNetworkAccessManager::finished, this, &MainWindow::HandleResponse);
}

MainWindow::~MainWindow() {
    delete ui;
    delete deviceManager;
}

void MainWindow::AddDevice() const {
    qDebug() << "add device";

    foreach(const auto &action, deviceManager->deviceList.last().actions){
        ui->lst_devices->addItem(action.description);
    }
}

void MainWindow::RemoveDevice(const int index) const {
    qDebug() << "remove device from ui at index " << index;
    RedrawList();
}

void MainWindow::on_lst_devices_clicked(const QModelIndex &index) const {
    qDebug() << "row " << index.row() << " selected";
    qDebug() << "max index : " << deviceManager->deviceList.length();
    //if(index.row() > 0 && index.row() <= deviceManager->deviceList.length()) {
        QNetworkRequest req = deviceManager->deviceList.at(index.row()).actions.first().req;

        mngr->get(req);
    //}
    //else {
    //    qDebug() << "clicked index out of range max index : " << deviceManager->deviceList.length();
    //}
}

void MainWindow::HandleResponse(QNetworkReply *reply) {
    if(reply->error()) {
       qDebug() << reply->errorString();
    }

    qDebug() << "handling call response" << reply->readAll();
}

void MainWindow::RedrawList() const{
    //TODO
    qDebug() << "redrawing device list";

    ui->lst_devices->clear();
    foreach(const auto &device, deviceManager->deviceList){
        foreach(const auto &action, device.actions){
            ui->lst_devices->addItem(action.description);
        }
    }
}
