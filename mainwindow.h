#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "devicelist.h"

#include <QBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

};

#endif // MAINWINDOW_H
