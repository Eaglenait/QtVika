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
