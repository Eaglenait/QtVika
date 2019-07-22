#ifndef ACTION_H
#define ACTION_H

#include <QJsonObject>
#include <QVector>
#include <QNetworkAccessManager>
#include "syntaxhelper.h"

enum ActionType {
    Toggle
    ,Analog
    ,Measure
    ,Undefined
};

class Action
{
public:
    Action();
    Action(ActionType Type, VikaSyntax Syntax, QString Uri, QString Description);

    bool operator==(const Action &) const;

    ActionType type;
    QString description;
    VikaSyntax syntax;

    QNetworkRequest req;

    void Call(QNetworkAccessManager &mngr, int arg = -1) const;

};
#endif // ACTION_H
