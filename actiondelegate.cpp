#include <QPainter>
#include "actiondelegate_p.h"
#include "actiondelegate.h"

ActionDelegate::ActionDelegate(QObject *parent) :
    QStyledItemDelegate (parent)
  , m_deviceInfo(new ActionDelegatePrivate())
{ }

ActionDelegate::~ActionDelegate() {
    delete m_deviceInfo;
}

QMargins ActionDelegate::contentsMargins() const {
    return m_deviceInfo->margins;
}

void ActionDelegate::setContentsMargins(int left, int top, int right, int bottom) const {
    m_deviceInfo->margins = QMargins(left, top, right, bottom);
}

int ActionDelegate::horizontalSpacing() const {
    return m_deviceInfo->spacingHorizontal;
}

void ActionDelegate::setHorizontalSpacing(int spacing) const {
    m_deviceInfo->spacingHorizontal = spacing;
}

int ActionDelegate::verticalSpacing() const {
   return m_deviceInfo->spacingVertical;
}

void ActionDelegate::setVerticalSpacing(int spacing) const {
    m_deviceInfo->spacingVertical = spacing;
}

void ActionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
   QStyleOptionViewItem opt(option);
   initStyleOption(&opt, index);

   const QPalette &palette(opt.palette);
   const QRect &rect(opt.rect);
   const QRect &contentRect(rect.adjusted(m_deviceInfo->margins.left(),
                                          m_deviceInfo->margins.top(),
                                          -m_deviceInfo->margins.right(),
                                          -m_deviceInfo->margins.bottom()));

   const bool lastIndex = (index.model()->rowCount() - 1) == index.row();
   const int bottomEdge = rect.bottom();
   QFont f(opt.font);

   f.setPointSize(m_deviceInfo->fontSize(opt.font));

   painter->save();
   painter->setClipping(true);
   painter->setClipRect(rect);
   painter->setFont(opt.font);

   //Draw background
   painter->fillRect(rect, opt.state & QStyle::State_Selected ?
                                                palette.highlight().color() :
                                                palette.light().color());

   //bottom line
   painter->setPen(lastIndex ? palette.dark().color()
                             : palette.mid().color());

   painter->drawLine(lastIndex ? rect.left() : m_deviceInfo->margins.left(),  bottomEdge, rect.right(), bottomEdge);

   //Draw description
   QRect descriptionRect(m_deviceInfo->descriptionBox(opt));

   //top left position
   descriptionRect.moveTo(m_deviceInfo->margins.left() + m_deviceInfo->spacingHorizontal, contentRect.top());

   painter->setFont(opt.font);
   painter->setPen(palette.windowText().color());
   painter->drawText(descriptionRect, Qt::TextSingleLine, opt.text);

   //Draw keywords
   QRect keyWordRect(m_deviceInfo->keyWordsBox(opt));
   //bottom left position
   keyWordRect.moveTo(m_deviceInfo->margins.left() + m_deviceInfo->spacingHorizontal + 2, contentRect.bottom() - keyWordRect.height());

   painter->setFont(opt.font);
   painter->setPen(palette.text().color());
   painter->drawText(keyWordRect, Qt::TextSingleLine, index.data(Qt::UserRole).toString());
   //Draw state
   QRect stateRect(m_deviceInfo->stateBox(opt));
   //top right position
   stateRect.moveTo(contentRect.width() - stateRect.width(), contentRect.top());

   painter->setFont(opt.font);
   painter->setPen(palette.windowText().color());
   painter->drawText(stateRect, Qt::TextSingleLine, index.data(Qt::UserRole + 1).toString());

   painter->restore();
}

QSize ActionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    int textHeight = m_deviceInfo->descriptionBox(opt).height()
                    + m_deviceInfo->spacingVertical
                    + m_deviceInfo->keyWordsBox(opt).height();

    return QSize(opt.rect.width(), m_deviceInfo->margins.top() + textHeight + m_deviceInfo->margins.bottom());
}

ActionDelegatePrivate::ActionDelegatePrivate() :
  margins(0, 0, 0, 0)
  , spacingHorizontal(0)
  , spacingVertical(0)
{}

QRect ActionDelegatePrivate::descriptionBox(const QStyleOptionViewItem &option) const {
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}

QRect ActionDelegatePrivate::keyWordsBox(const QStyleOptionViewItem &option) const {
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}

QRect ActionDelegatePrivate::stateBox(const QStyleOptionViewItem &option) const {
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}

qreal ActionDelegatePrivate::fontSize(const QFont &font) const {
    return 0.85 * font.pointSize();
}
