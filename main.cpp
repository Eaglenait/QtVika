#include "mainwindow.h"
#include <QApplication>
#include "devicemanager.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Q_INIT_RESOURCE(resources);

  MainWindow w;

  if(QSysInfo::productType() == "android")
  {
      w.setWindowFlag(Qt::MaximizeUsingFullscreenGeometryHint, true);

      //QScreen s = new QScreen() ;
      //w.size();
  }

  w.show();

  return a.exec();
}
