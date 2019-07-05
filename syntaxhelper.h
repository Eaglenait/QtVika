#ifndef SYNTAXHELPER_H
#define SYNTAXHELPER_H

#include <QString>
#include <QVector>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include "vikaword.h"
#include <QElapsedTimer>

class VikaSyntax {
public:
    VikaSyntax();
    VikaSyntax(QVector<QString> verbs, QVector<QString> objects, QString localisation, QVector<QString> adjectives);

    QVector<QString> Verbs;
    QVector<QString> Objects;
    QVector<QString> Adjectives;
    QString Localisation;
    QString description;

    void Print() const;

    //Compares two syntaxes and returns a equality score (0 - no match, 100 - Full match)
    static short Compare(const VikaSyntax& lhs, const VikaSyntax& rhs);
};

enum SyntaxType { Verb, Object, Localisation, Adjective };

//Interface between the json files and the objects
class SyntaxHelper {
public:
    SyntaxHelper();

    VikaSyntax GetSyntax(QStringList sentence);

    QVector<int> GetSynonyms(const QString& word, SyntaxType type) const;
    bool IsVerb(const QString& word) const;
    bool IsObject(const QString& word) const;
    bool IsLocalisation(const QString& word) const;
    bool isAdjective(const QString& word) const;

private:
    const QVector<QString> _excludedList = {"le", "la", "les"};

    QVector<VikaWord> _verbList;
    QVector<VikaWord> _objectList;
    QVector<VikaWord> _localisationList;
    QVector<VikaWord> _adjectiveList;

    bool HasWord(const QString &word, SyntaxType type) const;
};
#endif // SYNTAXHELPER_H
