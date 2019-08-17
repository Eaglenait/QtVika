#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>

#include <QNetworkAccessManager>
#include "devicemanager.h"

#include "device.h"
#include "syntaxhelper.h"
#include "QHttpMultiPart"

#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void AddDevice() const;
    void RemoveDevice(const int index) const;
    void HandleResponse(QNetworkReply *reply);

    void on_lst_devices_clicked(const QModelIndex &index) const;

private:
    Ui::MainWindow *ui;
    DeviceManager *deviceManager;
    SyntaxHelper *syntaxHelper;

    QNetworkAccessManager *mngr;
    QString syntax;
};

#endif // MAINWINDOW_H
