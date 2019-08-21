#ifndef DEVICEDELEGATE_H
#define DEVICEDELEGATE_H

#include <QStyledItemDelegate>

class ActionDelegatePrivate;

class ActionDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ActionDelegate(QObject *parent = nullptr);
    ~ActionDelegate() override;

    QMargins contentsMargins() const;
    void setContentsMargins(int left, int top, int right, int bottom) const;

    int horizontalSpacing() const;
    void setHorizontalSpacing(int spacing) const;

    int verticalSpacing() const;
    void setVerticalSpacing(int spacing) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    ActionDelegatePrivate *m_deviceInfo;
};

#endif // DEVICEDELEGATE_H
