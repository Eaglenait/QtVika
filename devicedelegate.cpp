#include <QPainter>
#include "devicedelegate_p.h"
#include "devicedelegate.h"

DeviceDelegate::DeviceDelegate(QObject *parent) :
    QStyledItemDelegate (parent)
  , m_deviceInfo(new DeviceDelegatePrivate())
{ }

DeviceDelegate::~DeviceDelegate() {
    delete m_deviceInfo;
}

QMargins DeviceDelegate::contentsMargins() const {
    return m_deviceInfo->margins;
}

void DeviceDelegate::setContentsMargins(int left, int top, int right, int bottom) const {
    m_deviceInfo->margins = QMargins(left, top, right, bottom);
}

int DeviceDelegate::horizontalSpacing() const {
    return m_deviceInfo->spacingHorizontal;
}

void DeviceDelegate::setHorizontalSpacing(int spacing) const {
    m_deviceInfo->spacingHorizontal = spacing;
}

int DeviceDelegate::verticalSpacing() const {
   return m_deviceInfo->spacingVertical;
}

void DeviceDelegate::setVerticalSpacing(int spacing) const {
    m_deviceInfo->spacingVertical = spacing;
}

void DeviceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
   QStyleOptionViewItem opt(option);
   initStyleOption(&opt, index);

   const QPalette &palette(opt.palette);
   const QRect &rect(opt.rect);
   const QRect &contentRect(rect.adjusted(m_deviceInfo->margins.left(),
                                          m_deviceInfo->margins.top(),
                                          m_deviceInfo->margins.right(),
                                          m_deviceInfo->margins.bottom()));

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
   descriptionRect.moveTo(m_deviceInfo->margins.left() + m_deviceInfo->spacingHorizontal,
                          contentRect.top() + m_deviceInfo->spacingVertical);

   painter->setFont(f);
   painter->setPen(palette.text().color());
   painter->drawText(descriptionRect, Qt::TextSingleLine, opt.text);

   //Draw keywords
   QRect keyWordRect(m_deviceInfo->keyWordsBox(opt));
   //bottom left position
   keyWordRect.moveTo(m_deviceInfo->margins.left() + m_deviceInfo->spacingHorizontal,
                      contentRect.bottom() - m_deviceInfo->spacingVertical);

   painter->setFont(opt.font);
   painter->setPen(palette.windowText().color());
   painter->drawText(keyWordRect, Qt::TextSingleLine, opt.text);

   //Draw state
   QRect stateRect(m_deviceInfo->stateBox(opt));
   //top right position
   stateRect.moveTo(m_deviceInfo->margins.right() - stateRect.width(),
                    contentRect.top() - m_deviceInfo->spacingVertical);

   painter->setFont(opt.font);
   painter->setPen(palette.windowText().color());
   painter->drawText(stateRect, Qt::TextSingleLine, opt.text);

   painter->restore();
}

QSize DeviceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    int textHeight = m_deviceInfo->descriptionBox(opt).height()
                    + m_deviceInfo->spacingVertical
                    + m_deviceInfo->keyWordsBox(opt).height();

    return QSize(opt.rect.width(), m_deviceInfo->margins.top() + textHeight + m_deviceInfo->margins.bottom());
}

DeviceDelegatePrivate::DeviceDelegatePrivate() :
  margins(0, 0, 0, 0)
  , spacingHorizontal(0)
  , spacingVertical(0)
{}

QRect DeviceDelegatePrivate::descriptionBox(const QStyleOptionViewItem &option) const {
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}

QRect DeviceDelegatePrivate::keyWordsBox(const QStyleOptionViewItem &option) const {
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}

QRect DeviceDelegatePrivate::stateBox(const QStyleOptionViewItem &option) const {
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}

qreal DeviceDelegatePrivate::fontSize(const QFont &font) const {
    return 0.85 * font.pointSize();
}
