#include <QCoreApplication>

#include "syntaxhelper.h"
#include "devicemanager.h"
#include <QtCore/QCoreApplication>
#include <QTextStream>
#include <QVector>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  Q_INIT_RESOURCE(resources);
  SyntaxHelper sh;
  DeviceManager *dm = new DeviceManager();
  EchoServer *server = new EchoServer(80, true);
  QObject::connect(server, &EchoServer::closed, &a, &QCoreApplication::quit);

  QTextStream qtin(stdin);

  while(true) {
    QStringList sentence = qtin.readLine().split(' ');
    if(sentence.count()== 1){
      if(sentence[0] == "discover"){
        dm->AdvertiseServer();
        continue;
      }
      if(sentence[0] == "exit") {
        return 0;
      }
      if(sentence[0].startsWith("getconfig")) {
        QVector<VikaSyntax> sy;
        if(sentence[0].contains(';')) {
          qDebug() << "split addr";
          auto args = sentence[0].split(';');
          QHostAddress addr = QHostAddress(args[1]);
          sy = dm->GetConfig(addr);
        } else {
          QHostAddress addr("10.22.2.101");
          sy = dm->GetConfig(addr);
        }

        qDebug() << "Printing config";
        foreach(const auto & config, sy) {
          config.Print();
        }
      }
    }

    auto s = sh.GetSyntax(sentence);
    s.Print();

    QStringList sentence2 = qtin.readLine().split(' ');
    auto s2 = sh.GetSyntax(sentence2);
    s2.Print();

    auto score = VikaSyntax::Compare(s,s2);
    if(score >= 210) {
    }

    qDebug() << "syntax score " << score << "/255";
  }

  return a.exec();
}
