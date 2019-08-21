#ifndef ACTIONDELEGATE_P_H
#define ACTIONDELEGATE_P_H

#include <qglobal.h>
#include <QStyleOptionViewItem>

class ActionDelegatePrivate {
    ActionDelegatePrivate();

    inline qreal fontSize(const QFont &font) const;

    inline QRect descriptionBox(const QStyleOptionViewItem &option) const;
    inline QRect keyWordsBox(const QStyleOptionViewItem &option) const;
    inline QRect stateBox(const QStyleOptionViewItem &option) const;

    QSize iconSize;
    QMargins margins;
    int spacingHorizontal;
    int spacingVertical;

    friend class ActionDelegate;
};

#endif // ACTIONDELEGATE_P_H
