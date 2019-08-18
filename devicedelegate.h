#ifndef DEVICEDELEGATE_H
#define DEVICEDELEGATE_H

#include <QStyledItemDelegate>

class DeviceDelegatePrivate;

class DeviceDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DeviceDelegate(QObject *parent = 0);
    ~DeviceDelegate();

    QMargins contentsMargins() const;
    void setContentsMargins(int left, int top, int right, int bottom) const;

    int horizontalSpacing() const;
    void setHorizontalSpacing(int spacing) const;

    int verticalSpacing() const;
    void setVerticalSpacing(int spacing) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    DeviceDelegatePrivate *m_deviceInfo;
};

#endif // DEVICEDELEGATE_H
