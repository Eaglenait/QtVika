#ifndef DEVICE_H
#define DEVICE_H

#include <QHostAddress>
#include "action.h"
#include <QVector>

class Device {
public:
    Device(const QHostAddress &Address,
           const QVector<Action> &actions);

    //REST call to the device to get its config
    static Device GetConfig(const QHostAddress &Address);

    bool HasAction(const Action &action) const;

private:
    QHostAddress _address;
    QVector<Action> _actions;
};

#endif // DEVICE_H
