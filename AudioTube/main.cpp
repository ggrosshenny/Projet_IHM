#include "mainwindow.h"
#include "serveur.h"
#include "sendjsoncommandToMPV.h"
#include <unistd.h>
#include <QObject>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    // Creation serveur
    Serveur *serv = new Serveur();
    SendjsoncommandToMPV *cmd = new SendjsoncommandToMPV();

    //cmd->changeMusicOnMPV("test.mp3");
    serv->changeMusique("test.mp3");

    unsigned int ms = 2;

    sleep(ms);

    qDebug() << "On met en pause";
    cmd->setPauseOnMPV(true);

    sleep(ms);

    qDebug() << "On met play";
    cmd->setPauseOnMPV(false);

    return a.exec();
}
