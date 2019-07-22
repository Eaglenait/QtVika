#include "action.h"

Action::Action(){}

Action::Action(ActionType Type, VikaSyntax Syntax, QString Uri, QString Description) :
    type(Type)
  , description(Description)
  , syntax(Syntax)
{
    req.setUrl(Uri);
}
bool Action::operator==(const Action &action) const{
    if(VikaSyntax::Compare(action.syntax, this->syntax) == 100) {
        return true;
    }
    return false;
}

void Action::Call(QNetworkAccessManager &mngr, int arg) const {
    //TODO handle analog call and measure
    //arg has to be called "arg"
    qDebug() << "Action - calling";

    mngr.get(req);
}
