#include "devicelist.h"

DeviceList::DeviceList(QWidget *parent):
    QListView (parent)
{
    auto *deviceDelegate = new DeviceDelegate(this);
    QPalette p(palette());

    p.setBrush(QPalette::WindowText,    QColor("#303030"));
    p.setBrush(QPalette::Base,          QColor("#F0F1F2"));
    p.setBrush(QPalette::Light,         QColor("#FFFFFF"));
    p.setBrush(QPalette::Midlight,      QColor("#D3D6D8"));
    p.setBrush(QPalette::Mid,           QColor("#C5C9CB"));
    p.setBrush(QPalette::Dark,          QColor("#9AA0A4"));
    p.setBrush(QPalette::Text,          QColor("#616B71"));
    p.setBrush(QPalette::Highlight,     QColor("#E2E4E5"));

    deviceDelegate->setContentsMargins(10, 10, 10, 10);
    deviceDelegate->setHorizontalSpacing(10);
    deviceDelegate->setVerticalSpacing(10);

    setPalette(p);
    setFont(QFont("MSShellDlg 2", 12));
    setModel(new QStandardItemModel(this));

    setItemDelegate(deviceDelegate);
}

void DeviceList::AddDevice(const QString &description, const QString &keyword, const QString &state) {
    auto *item = new QStandardItem(description);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(keyword, Qt::UserRole);
    item->setData(state, Qt::UserRole);

    static_cast<QStandardItemModel *>(model())->appendRow(item);
}

void DeviceList::ClearAll() {
    static_cast<QStandardItemModel *>(model())->clear();
}
