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
    qDebug() << "row " << index.row() << " selected";
    QNetworkRequest req = deviceManager->deviceList.at(index.row()).actions.first().req;

    mngr->get(req);
}

void MainWindow::HandleResponse(QNetworkReply *reply){
    if(reply->error()) {
       qDebug() << reply->errorString();
    }

    qDebug() << "handling call response" << reply->readAll();
}

void MainWindow::on_btn_syntax_clicked()
{
    qDebug() << "MainWindow - Getting syntax";

    VikaSyntax s = syntaxHelper->GetSyntax(syntax.split(' '));
    foreach(const auto &device, deviceManager->deviceList) {
        foreach(const auto &action, device.actions) {
            short compareScore = VikaSyntax::Compare(s, action.syntax);
            qDebug() << "Compare score: " << compareScore;
            if(compareScore >= 70) {
                mngr->get(action.req);
                return;
            }
        }
    }
}

void MainWindow::on_syntax_textEdited(const QString &arg1)
{
    syntax = arg1;
}
