#ifndef ACTION_H
#define ACTION_H

#include <QJsonObject>
#include <QVector>

enum ActionType {
    Toggle,
    Analog,
    Measure
};

struct Syntax{
public:
    //TODO REMOVE
    Syntax(){

    }
    Syntax(const Syntax &syntax) {
        this->_verbs = syntax._verbs;
        this->_objects = syntax._objects;
        this->_localisations = syntax._localisations;
    }

private:
    QVector<QString> _verbs;
    QVector<QString> _objects;
    QVector<QString> _localisations;
};

class Action
{
public:
    Action(ActionType Type, const Syntax &Syntax, const QString &Uri);


    /// \brief Calls the action
    /// \param optional value
    /// \return json with response code or with value depending on call type
    QJsonObject Call(int value = -1);

private:
    ActionType _type;
    Syntax _syntax;


    QString _uri;


};

#endif // ACTION_H
