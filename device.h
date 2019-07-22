#ifndef DEVICE_H
#define DEVICE_H

#include <QHostAddress>
#include "action.h"
#include <QVector>

class Device {
public:
    Device(QHostAddress Address, QVector<Action> actions);

    QHostAddress address;
    QVector<Action> actions;
    int IndexOfAction(const Action &action);

    void Print() const;

};

#endif // DEVICE_H
