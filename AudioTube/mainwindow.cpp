#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "codecommandes.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ServerSocket(new QLocalSocket(this))
{
    ui->setupUi(this);

    // Connexion au serveur MPV
    ServerSocket->connectToServer("/tmp/server-IHM");

    connect(ServerSocket, SIGNAL(readyRead()), this, SLOT(readSocketOnServer()));

    // Verify if we are connected
    if(ServerSocket->waitForConnected())
    {
        qDebug() << "Connected to mpv server.";
    }
    else
    {
        ServerSocket->error();
    }

    jsonToSrv = new SendJSONCommandToServer(0, "/tmp/server-IHM");

    // Connects entre les messages reçus du serveur et l'UI
    QObject::connect(this, SIGNAL(setVolume(int)), this, SLOT(changeVolume(int)));
    QObject::connect(this, SIGNAL(setoffsetMusique(int)), this, SLOT(changePositionOffset(int)));
    QObject::connect(this, SIGNAL(setTempsTotal(int)), this, SLOT(changeTempsTotal(int)));
    QObject::connect(this, SIGNAL(setTempsEcoule(int)), this, SLOT(changeTempsEcoule(int)));

    // Connects des messages venant de l'ui pour l'envoie au serveur
    QObject::connect(ui->Play, SIGNAL(pressed()), this, SLOT(setPlay()));
    QObject::connect(ui->Arret, SIGNAL(pressed()), this, SLOT(setStop()));
    QObject::connect(ui->Pause, SIGNAL(pressed()), this, SLOT(setPause()));
    QObject::connect(ui->AvanceRapide, SIGNAL(pressed()), this, SLOT(setAR()));
    QObject::connect(ui->RetourRapide, SIGNAL(pressed()), this, SLOT(setRR()));
    QObject::connect(ui->buttonQuit, SIGNAL(pressed()), this, SLOT(quit()));
    QObject::connect(ui->ControleVolume, SIGNAL(valueChanged(int)), this, SLOT(setVolumeOnServer(int)));
    QObject::connect(ui->PointeurTempsMusique, SIGNAL(valueChanged(int)), this, SLOT(setOffsetOnServer(int)));

}

MainWindow::~MainWindow()
{
    m_socket->disconnectFromServer();
    delete ui;
}


// ------------------------------------------------------
// Du client à l'UI


/**
 * Slot : changeVolume
 * Param : int, volume - novueau volume à ajouter
 * Desc : change la valeur du slider du son sur l'ui
 * Return : void
 **/
void MainWindow::changeVolume(int volume)
{
    ui->ControleVolume->setValue(volume);
    ui->VolumeAffichage->setText(QString::number(volume));
}


/**
 * Slot : changePositionOffset
 * Param : int, offset - nouvel offset de la musique
 * Desc : change l'offset du morceau en cours sur l'UI
 * Return : void
 **/
void MainWindow::changePositionOffset(int offset)
{
    ui->PointeurTempsMusique->setValue(offset);
}


/**
 * Method : changeTempsTotal
 * Param : int, tempsTotal - temps total de la musique en cours
 * Desc : change le temps total sur l'UI
 * Return : void
 **/
void MainWindow::changeTempsTotal(int tempsTotal)
{
    int tpsMinutes = tempsTotal/60;
    int tpsSecondes = tempsTotal%60;
    QString tempsTotalString = QString(QString::number(tpsMinutes) + ":" + QString::number(tpsSecondes));
    ui->TempsTotal->setText(tempsTotalString);
}


/**
 * Slot : changeTempsEcoule
 * Param : int, tempsEcoule - temps écoulé depuis le lancement de la musique
 * Desc : Change l'affichage du temps écoulé sur l'UI.
 * Return : void
 **/
void MainWindow::changeTempsEcoule(int tempsEcoule)
{
    int tpsMinutes = tempsEcoule/60;
    int tpsSecondes = tempsEcoule%60;
    QString tempsTotalString = QString(QString::number(tpsMinutes) + ":" + QString::number(tpsSecondes));
    ui->TempsCourant->setText(tempsTotalString);
}


/**
 * Method : readSocketOnServer
 * Param : void
 * Desc : Lis le message entrant sur la socket du serveur quand le signal readyRead est reçu
 * Return : void
 **/
void MainWindow::readSocketOnServer() {

    while(ServerSocket->canReadLine()){

        // parsing du JSON

        int valeur = 0;
        QByteArray readLineFromServer = ServerSocket->readLine().trimmed();  // Prendre JSon du socket
        QJsonParseError error;
        QJsonDocument jDoc = QJsonDocument::fromJson(readLineFromServer, &error);
        QJsonObject jObj = jDoc.object();
        QJsonArray jArr = jObj["command"].toArray();
        int cmd = jArr.at(0).toInt();

        qDebug() << "la commande reçu est : " << cmd;
        // Utilisation du JSON

        // Si la commande est ...

        switch(cmd)
        {
            case changeVolumeCMD:
                valeur = static_cast<int>(jArr.at(1).toDouble());
                qDebug() << "la commande reçu est : changeVolumeCMD de valeur : " << valeur;
                emit setVolume(valeur);
                break;

            case changeOffsetCMD:
                valeur = static_cast<int>(jArr.at(1).toDouble());
                qDebug() << "la commande reçu est : changeOffsetCMD de valeur : " << valeur;
                emit setoffsetMusique(valeur);
                break;

            case changeCurrentTimeCMD:
                valeur = static_cast<int>(jArr.at(1).toDouble());
                qDebug() << "la commande reçu est : changeCurrentTimeCMD de valeur : " << valeur;
                emit setTempsEcoule(valeur);
                break;

            case changeTotalTimeCMD:
                valeur = static_cast<int>(jArr.at(1).toDouble());
                qDebug() << "la commande reçu est : changeTotalTimeCMD de valeur : " << valeur;
                emit setTempsTotal(valeur);
                break;
        }

    }

}


// -----------------------------------------
// Du client vers le serveur


/**
 * Method : setPlay
 * Param : void
 * Desc : change l'état du serveur en play en envoyant la demande au serveur
 * Return : void
 */
void MainWindow::setPlay()
{
    jsonToSrv->SendPlayToServ();
}


/**
 * Method : setPause
 * Param : void
 * Desc : change l'état du serveur en pause en envoyant la demande au serveur
 * Return : void
 */
void MainWindow::setPause()
{
    jsonToSrv->SendPauseToServ();
}


/**
 * Method : setStop
 * Param : void
 * Desc : change l'état du serveur en stop en envoyant la demande au serveur
 * Return : void
 */
void MainWindow::setStop()
{
    jsonToSrv->SendStopToServ();
}


/**
 * Method : setAR
 * Param : void
 * Desc : change l'état du serveur en avanceRapide en envoyant la demande au serveur
 * Return : void
 */
void MainWindow::setAR()
{
    jsonToSrv->SendFastForwardToServ();
}


/**
 * Method : setRR
 * Param : void
 * Desc : change l'état du serveur en retourRapide en envoyant la demande au serveur
 * Return : void
 */
void MainWindow::setRR()
{
    jsonToSrv->SendFastReturnToServ();
}


/**
 * Method : setRR
 * Param : void
 * Desc : change l'état du serveur en retourRapide en envoyant la demande au serveur
 * Return : void
 */
void MainWindow::setVolumeOnServer(int volume)
{
    jsonToSrv->SendVolumeToServ(static_cast<double>(volume));
}


/**
 * Method : quit
 * Param : void
 * Desc : change l'état du serveur en quit en envoyant la demande au serveur
 * Return : void
 */
void MainWindow::quit()
{
    jsonToSrv->sendQuitToServer();
}


/**
 *
 *
 *
 *
 **/
void MainWindow::setOffsetOnServer(int offset)
{
    jsonToSrv->sendOffsetToServer(offset);
}
