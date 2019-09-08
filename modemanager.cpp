#include "modemanager.h"

ModeManager::ModeManager(QWidget *parent)
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

    resize(parent->size());
    setItemDelegate(actionDelegate);

    QByteArray jsonData;
    QJsonParseError parseError;
    QFile modesFile(":/modes.json");

    modesFile.open(QIODevice::ReadOnly | QIODevice::Text);
    modesFile.close();

    QJsonDocument document = QJsonDocument::fromJson(jsonData, &parseError);

    if(parseError.error != QJsonParseError::NoError) {
        qDebug() << "Parse error (verb): " << parseError.errorString();
        return;
    }
    data = new QJsonDocument(document);
}

void ModeManager::AddMode(const Mode &mode) {
    auto *item = new QStandardItem(mode.description);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    item->setData("",           Qt::UserRole);
    item->setData("",      		Qt::UserRole + 1);
    item->setData(mode.uris, 	Qt::UserRole + 2);

    static_cast<QStandardItemModel *>(model())->appendRow(item);
}

void ModeManager::SaveModes() {
    QFile modesFile(":/modes.json");

    modesFile.open(QIODevice::WriteOnly | QIODevice::Text);
    modesFile.write(data->toBinaryData());
    modesFile.close();
}
