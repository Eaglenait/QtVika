#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include <QListView>
#include <QWidget>
#include <QStandardItemModel>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include "mode.h"
#include "action.h"
#include "actiondelegate.h"

class ModeManager : public QListView
{
    Q_OBJECT
public:

    explicit ModeManager(QWidget *parent = Q_NULLPTR);

    void AddMode(const Mode &);
    void RemoveMode(const int &index);

    void CallMode() const;

private:
    QJsonDocument *data;

    void SaveModes();
};

#endif // MODEMANAGER_H
