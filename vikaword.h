#ifndef VIKAWORD_H
#define VIKAWORD_H

#include <QVector>

class VikaWord {
public:
    VikaWord() {
        this->id = 0;
        this->word = "";
        this->synonyms = QVector<QString>(1);
    }
    VikaWord(const VikaWord &vikaWord) { id = vikaWord.id; word = vikaWord.word; synonyms = vikaWord.synonyms; }

    int id;
    QString word;
    QVector<QString> synonyms;
};

#endif // VIKAWORD_H
