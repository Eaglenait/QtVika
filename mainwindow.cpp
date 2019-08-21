#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    auto *layoutMain = new QVBoxLayout(this);
    auto *actionList = new ActionList(this);

    deviceManager = new DeviceManager(this);

    //Add devices actions to UI
    QObject::connect(deviceManager, &DeviceManager::DeviceDiscovered,
                     actionList, &ActionList::AddActions);

    QObject::connect(actionList, &ActionList::clicked,
                     deviceManager, &DeviceManager::CallAction);

    layoutMain->addWidget(actionList);

    resize(800,600);
}

