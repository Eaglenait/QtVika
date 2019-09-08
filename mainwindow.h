#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "actionlist.h"
#include <QObject>

#include <QBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMainWindow>

#include "devicemanager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    DeviceManager *deviceManager;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
