#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    auto *layoutMain = new QVBoxLayout(this);
    auto *deviceList = new DeviceList(this);

    deviceList->AddDevice("description 1"
                          , "keyword a"
                          , "state");
    deviceList->AddDevice("description 2"
                          , "keyword b"
                          , "state");
    deviceList->AddDevice("description 3"
                          , "keyword c"
                          , "state");

    layoutMain->addWidget(deviceList);

    resize(800,600);
}

