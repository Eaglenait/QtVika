#include "device.h"
Device::Device(QHostAddress Address,QVector<Action> Actions):
    address(Address)
  , actions(Actions)
{
}

int Device::IndexOfAction(const Action &action) {
    for (int i = 0; i < actions.length(); ++i) {
        if(action == actions[i]){
           return i;
        }
    }
    return -1;
}

void Device::Print() const {
    qDebug()<< "Device: " << this->address;
    foreach(const auto &action, actions){
        qDebug() << "ActionType: " << action.type << " description " << action.description;
        action.syntax.Print();
    }
}
