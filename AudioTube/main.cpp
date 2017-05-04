#include "mainwindow.h"
#include "serveur.h"
#include "sendjsoncommandToMPV.h"
#include <unistd.h>
#include <QObject>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Creation serveur
    Serveur *serv = new Serveur();

    // Création du client et de son UI
    MainWindow w(0);
    w.show();


    //SendjsoncommandToMPV *cmd = new SendjsoncommandToMPV();

    //cmd->changeMusicOnMPV("test.mp3");
    serv->onChangeMusique("test.mp3");


    //unsigned int ms = 10;

    //sleep(ms);

    //qDebug() << "On arrete le serveur MPV";
    //cmd->quitMPVServer();

    /**
    qDebug() << "On met en pause";
    cmd->setPauseOnMPV(true);

    sleep(ms);

    qDebug() << "On met play";
    cmd->setPauseOnMPV(false);
    **/
    return a.exec();
}
