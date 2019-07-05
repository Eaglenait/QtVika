#include "action.h"

Action::Action(ActionType Type, const Syntax &Syntax, const QString &Uri) {
    _type = Type;
    _uri = Uri;

    _syntax = Syntax;
}

QJsonObject Action::Call(int value) {
    if(value != -1){

        //call uri without value

    }else{

        //call uri with value

    }

    //handle response JSON

    QJsonObject j;
    return j;
}
