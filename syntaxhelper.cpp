#include "syntaxhelper.h"

VikaSyntax::VikaSyntax()
    : Verbs(), Objects(), Localisation()
{}

VikaSyntax::VikaSyntax(QVector<QString> verbs, QVector<QString> objects, QString localisation, QVector<QString> adjectives)
    : Verbs(verbs)
    , Objects(objects)
    , Adjectives(adjectives)
    , Localisation(localisation)
{}

short VikaSyntax::Compare(const VikaSyntax& lhs, const VikaSyntax& rhs){
    //TODO: synonyms

    short score = 0;
    short grps = 0;

    //if has no verbs/obj/loc don't count in score
    if(lhs.Verbs.count() > 0) grps++;
    if(lhs.Objects.count() > 0) grps++;
    if(lhs.Localisation.length() > 0) grps++;
    if(lhs.Adjectives.count() > 0) grps++;
    if(grps == 0) return 0;

    if(lhs.Verbs.count() > 0 && rhs.Verbs.count() > 0) {
        foreach(auto &l_verb, lhs.Verbs) {
            foreach(auto &r_verb, rhs.Verbs) {
                if(l_verb.count() == r_verb.count()) {
                    if(l_verb == r_verb) {
                        score += (255/grps)/lhs.Verbs.count();
                    }
                }
            }
        }
    }

    if(lhs.Objects.count() > 0 && rhs.Objects.count() > 0) {
        foreach(auto &l_object, lhs.Objects) {
            foreach(auto &r_object, rhs.Objects) {
                if(l_object.split(';')[0].count() == r_object.split(';')[0].count()) {
                    if(l_object.split(';')[0] == r_object.split(';')[0]) {
                        score += (255/grps)/lhs.Objects.count();
                    }
                }
            }
        }
    }

    if(lhs.Localisation.length() > 0
      && rhs.Localisation.length() > 0
      && lhs.Localisation.length() == rhs.Localisation.length()) {
        score += (255/grps);
    }

    if(lhs.Adjectives.count() > 0 && rhs.Adjectives.count() > 0) {
        foreach(auto &l_Adjectives, lhs.Adjectives) {
            foreach(auto &r_Adjectives, rhs.Adjectives) {
                if(l_Adjectives.count() == r_Adjectives.count()) {
                    if(l_Adjectives == r_Adjectives) {
                        score += (255/grps)/lhs.Adjectives.count();
                    }
                }
            }
        }
    }

    return score;
}

void VikaSyntax::Print() const {
    qDebug() <<  "Syntax: ";

    if(Verbs.count() > 0){
        qDebug() << ' ' << "Verbs: ";
        foreach (auto & verb, Verbs) qDebug() << "  " << verb;
    } else {
        qDebug() << ' ' << "No Verbs";
    }

    if(Objects.count() > 0){
        qDebug() << ' ' << "Objects: ";
        foreach (auto & object, Objects) qDebug() << "  " << object;
    } else {
        qDebug() << ' ' << "No Objects";
    }

    if(Localisation.length() > 0){
        qDebug() << ' ' << "Localisation: " << Localisation;
    } else {
        qDebug() << ' ' << "No Localisations";
    }

    if(Adjectives.count() > 0){
        qDebug() << ' ' << "Adjectives: ";
        foreach (auto & adjective, Adjectives) qDebug() << "  " << adjective;
    } else {
        qDebug() << ' ' << "No Adjectives";
    }
}

/**************************************************/

SyntaxHelper::SyntaxHelper() {
    QFile verbFile(":/wordLists/verbsWordList.json");
    QFile objectFile(":/wordLists/objectsWordList.json");
    QFile localisationFile(":/wordLists/locationWordList.json");
    QFile adjectiveFile(":/wordLists/adjectiveWordList.json");

    QJsonDocument doc;
    QByteArray jsonData;
    QJsonParseError parseError;
    QJsonDocument document;
    QJsonArray array;

    QElapsedTimer timer;
    timer.start();

    //read verbs into verbList
    verbFile.open(QIODevice::ReadOnly | QIODevice::Text);
    jsonData = verbFile.readAll();
    verbFile.close();

    document = QJsonDocument::fromJson(jsonData, &parseError);
    jsonData.clear();

    if(parseError.error != QJsonParseError::NoError) {
        qDebug() << "Parse error (verb): " << parseError.errorString();
        return;
    }
    parseError.error = QJsonParseError::NoError;

    if(!document.isArray()){
        qDebug() << "Doc does not contain an array (verb)";
        return;
    }

    array = document.array();

    foreach(const QJsonValue & v,array){
        QJsonObject obj = v.toObject();

        VikaWord verb;
        verb.id = obj["id"].toInt();
        verb.word = obj["r"].toString();
        foreach(const QJsonValue & synonyms , obj["s"].toArray()){
            verb.synonyms.append(synonyms.toString());
        }

        _verbList.append(verb);
    }

    qDebug() << _verbList.count() << " verbs init in: " << timer.nsecsElapsed();
    timer.restart();

    //read objects into objectList
    objectFile.open(QIODevice::ReadOnly | QIODevice::Text);
    jsonData = objectFile.readAll();
    objectFile.close();
    document = QJsonDocument::fromJson(jsonData, &parseError);
    jsonData.clear();

    if(parseError.error != QJsonParseError::NoError) {
        qDebug() << "Parse error (object): " << parseError.errorString();
        return;
    }
    parseError.error = QJsonParseError::NoError;

    if(!document.isArray()){
        qDebug() << "Doc does not contain an array (object)";
        return;
    }

    array = document.array();

    foreach(const QJsonValue & o,array){
        QJsonObject jsonObj = o.toObject();

        VikaWord object;
        object.id = jsonObj["id"].toInt();
        object.word = jsonObj["n"].toString();
        foreach(const QJsonValue & synonyms , jsonObj["s"].toArray()){
            object.synonyms.append(synonyms.toString());
        }

        _objectList.append(object);
    }

    qDebug() << _objectList.count() << " objects init in: " << timer.nsecsElapsed();
    timer.restart();

    //read Localisations into localisationList
    localisationFile.open(QIODevice::ReadOnly | QIODevice::Text);
    jsonData = localisationFile.readAll();
    localisationFile.close();
    document = QJsonDocument::fromJson(jsonData, &parseError);
    jsonData.clear();

    if(parseError.error != QJsonParseError::NoError) {
        qDebug() << "Parse error (localisation): " << parseError.errorString();
        return;
    }
    parseError.error = QJsonParseError::NoError;

    if(!document.isArray()){
        qDebug() << "Doc does not contain an array (localisation)";
        return;
    }

    array = document.array();

    foreach(const QJsonValue & l,array){
        QJsonObject jsonObj = l.toObject();

        VikaWord localisation;
        localisation.id = jsonObj["id"].toInt();
        localisation.word = jsonObj["l"].toString();
        foreach(const QJsonValue & synonyms , jsonObj["s"].toArray()){
            localisation.synonyms.append(synonyms.toString());
        }

        _localisationList.append(localisation);
    }

    qDebug() << _localisationList.count() << " Localisations init in: " << timer.nsecsElapsed();
    timer.restart();

    //read adjective into adjectiveList
    adjectiveFile.open(QIODevice::ReadOnly | QIODevice::Text);
    jsonData = adjectiveFile.readAll();
    adjectiveFile.close();

    document = QJsonDocument::fromJson(jsonData, &parseError);
    jsonData.clear();

    if(parseError.error != QJsonParseError::NoError) {
        qDebug() << "Parse error (adjective): " << parseError.errorString();
        return;
    }
    parseError.error = QJsonParseError::NoError;

    if(!document.isArray()){
        qDebug() << "Doc does not contain an array (adjective)";
        return;
    }

    array = document.array();

    foreach(const QJsonValue & a,array){
        QJsonObject obj = a.toObject();

        VikaWord adjective;
        adjective.id = obj["id"].toInt();
        adjective.word = obj["a"].toString();
        foreach(const QJsonValue & synonyms , obj["s"].toArray()){
            adjective.synonyms.append(synonyms.toString());
        }

        _adjectiveList.append(adjective);
    }

    qDebug() << _adjectiveList.count() << " Adjectives init in: " << timer.nsecsElapsed();
    timer.restart();
}

bool SyntaxHelper::IsVerb(const QString& verb) const {
    return HasWord(verb, SyntaxType::Verb);
}

bool SyntaxHelper::IsObject(const QString& object) const {
    return HasWord(object, SyntaxType::Object);
}

bool SyntaxHelper::IsLocalisation(const QString& localisation) const {
    return HasWord(localisation, SyntaxType::Localisation);
}

bool SyntaxHelper::isAdjective(const QString& word) const {
    return HasWord(word, SyntaxType::Adjective);
}

bool SyntaxHelper::HasWord(const QString &word, SyntaxType type) const {
    if(_excludedList.contains(word)) {
        return false;
    }

    switch (type) {
        case SyntaxType::Verb:
        foreach(const VikaWord &verb, _verbList)
            if(word.startsWith(verb.word))
                return true;
        break;

        case SyntaxType::Object:
        foreach(const VikaWord &object, _objectList)
            if(word.startsWith(object.word))
                return true;
        break;

        case SyntaxType::Localisation:
        foreach(const VikaWord &localisation, _localisationList)
            if(word.startsWith(localisation.word)){
                return true;
            }
        break;

        case SyntaxType::Adjective:
        foreach(const VikaWord &adjective, _adjectiveList){
            if(word.startsWith(adjective.word)){
               return true;
            }
        }
        break;
    }
    return false;
}

VikaSyntax SyntaxHelper::GetSyntax(QStringList sentence){
    QVector<QString> Verbs;
    QVector<QString> Objects;
    QVector<QString> Adjectives;
    QString Localisation;

    int sentenceCount = sentence.count() - 1;
    for (int i = 0; i < sentence.count(); ++i) {
        if(IsVerb(sentence[i])) {
            Verbs.append(sentence[i]);
        }

        if(IsObject(sentence[i])) {
            Objects.append(sentence[i]);
        }

        if(isAdjective(sentence[i])) {
            Adjectives.append(sentence[i]);
        }

        //Localisation
        if(IsLocalisation(sentence[i])){
            //composite localisations (salle de bain)
            if((i+2) <= sentenceCount) {
                if(sentence[i+1] == "de"){
                    Localisation = sentence [i] + " de " + sentence[i+2];
                }
            } else {
                Localisation = sentence[i];
            }
        }
    }

    return VikaSyntax(Verbs, Objects, Localisation, Adjectives);
}
