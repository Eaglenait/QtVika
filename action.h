#ifndef ACTION_H
#define ACTION_H

#include <QJsonObject>
#include <QVector>
#include <QNetworkAccessManager>
#include "syntaxhelper.h"

#include "QHttpMultiPart"
#include "QHttpPart"

enum ActionType { Toggle, Analog, Measure, Undefined };

class Action {
public:
    Action();
    Action(ActionType Type, VikaSyntax Syntax, QString Uri, QString Description);

    bool operator==(const Action &) const;

    ActionType type;
    QString description;
    VikaSyntax syntax;

    QNetworkRequest req;
};
#endif // ACTION_H
