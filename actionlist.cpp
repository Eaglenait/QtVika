#include "actionlist.h"

ActionList::ActionList(QWidget *parent)
    : QListView(parent)
{
    auto *actionDelegate = new ActionDelegate(this);
    QPalette p(palette());

    p.setBrush(QPalette::WindowText,    QColor("#303030"));
    p.setBrush(QPalette::Base,          QColor("#F0F1F2"));
    p.setBrush(QPalette::Light,         QColor("#FFFFFF"));
    p.setBrush(QPalette::Midlight,      QColor("#D3D6D8"));
    p.setBrush(QPalette::Mid,           QColor("#C5C9CB"));
    p.setBrush(QPalette::Dark,          QColor("#9AA0A4"));
    p.setBrush(QPalette::Text,          QColor("#616B71"));
    p.setBrush(QPalette::Highlight,     QColor("#E2E4E5"));

    actionDelegate->setContentsMargins(10, 10, 10, 10);
    actionDelegate->setHorizontalSpacing(8);
    actionDelegate->setVerticalSpacing(4);

    setPalette(p);
    setFont(QFont("MSShellDlg 2", 12));
    setModel(new QStandardItemModel(this));

    resize(parent->size());

    setItemDelegate(actionDelegate);
}

void ActionList::AddActions(const Device &d) {
    foreach(const Action &a, d.actions) {
        qDebug() << "action added to UI";
        qDebug() << a.description;
        AddAction(a, "0");
    }
}

void ActionList::UpdateStatusAtIndex(const QModelIndex &index, const QString &status) {
    auto item = static_cast<QStandardItemModel *>(model())->itemFromIndex(index);
    item->setData(status, Qt::UserRole + 2);
}

void ActionList::AddAction(const Action &a, const QString &state) {
    QString kw = "";
    foreach(const QString &o, a.syntax.Objects) {
        kw.append(o);
        kw.append(' ');
    }

    auto *item = new QStandardItem(a.description);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    item->setData(kw,           Qt::UserRole);
    item->setData(state,        Qt::UserRole + 1);
    item->setData(a.req.url(),  Qt::UserRole + 2);

    static_cast<QStandardItemModel *>(model())->appendRow(item);
}

void ActionList::ClearAll() {
    static_cast<QStandardItemModel *>(model())->clear();
}
