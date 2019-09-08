#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    deviceManager = new DeviceManager(this);

    ui->setupUi(this);
    auto *actionList = new ActionList(ui->actionTab);

    //mock data
    auto ac = QVector<Action>();
    ac.push_back(Action(ActionType::Toggle, VikaSyntax(), "", "mock action 1"));
    ac.push_back(Action(ActionType::Toggle, VikaSyntax(), "", "mock action 2"));
    ac.push_back(Action(ActionType::Toggle, VikaSyntax(), "", "mock action 3"));

    Device d(QHostAddress(""), ac);
    actionList->AddActions(d);
    //

    //Add devices actions to UI
    QObject::connect(deviceManager, &DeviceManager::DeviceDiscovered,
                     actionList, &ActionList::AddActions);

    //on ui click, call action
    QObject::connect(actionList, &ActionList::clicked,
                     deviceManager, &DeviceManager::CallAction);

    //update status from action on UI
    QObject::connect(deviceManager, &DeviceManager::ActionCalled,
                     actionList, &ActionList::UpdateStatusAtIndex);

    qDebug() << "Mainwindow setted up";
}

MainWindow::~MainWindow() {
   delete ui;
}
