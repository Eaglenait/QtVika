#include "device.h"
Device::Device(const QHostAddress &Address,
               const QVector<Action> &Actions):
    _address(Address), _actions(Actions) {}

static Device GetConfig(const QHostAddress &Address) {
    //Rest call to ipAddress
    //Returns a json
    //parse JSON and init new device

}
