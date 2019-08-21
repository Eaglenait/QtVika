#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "actionlist.h"
#include <QObject>

#include <QBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

#include "devicemanager.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    DeviceManager *deviceManager;
};

#endif // MAINWINDOW_H
