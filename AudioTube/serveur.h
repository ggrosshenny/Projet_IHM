#ifndef SERVEUR_H
#define SERVEUR_H

#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QHistoryState>
#include <QFinalState>
#include <QSignalTransition>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "sendjsoncommandToMPV.h"
#include "codecommandes.h"


class Serveur : public QObject
{
    Q_OBJECT
public:
    Serveur(QObject *parent = 0, QString mpvSocket = "/tmp/mpv-socket");

    // Messages de modification de MPV reçus de l'UI
    void onChangeMusique(QString musique);

    void runObserves();


private:
    // Un object SendjsoncommandToMPV pour la communication vers MPV
    SendjsoncommandToMPV *communicationMPV;

    // Serveur local
    QLocalServer *clientServer = NULL;

    // Sockets des clients et de MPV
    QLocalSocket *MPVSocket = NULL;
    QList<QLocalSocket*> clientsSocket;

    // Machine à état
    QStateMachine *machine;

    // Etats de la machine
    QState *play;           // Etat où la musique est jouée
    QState *pause;          // Etat de pause
    QState *stop;           // Etat où l'offset de la musique est remise à zero
    QState *retourRapide;   // Etat de retour rapide
    QState *avanceRapide;   // Etat d'avance rapide

    // Etat final
    QFinalState *fin;       // Etat où l'on quitte MPV et on ferme le serveur


    // Transitions des états
    // De l'état play vers tous les autres états
    /*
        play -> pause        on versPause
        play -> retourRapide on versRR
        play -> avanceRapide on versAr
        play -> stop         on versStop
        play -> fin          on versQuit
    */


    // De l'état pause vers ...
    /*
        pause -> play on versPlay
        pause -> stop on versStop
        pause -> fin  on versQuit
    */


    // De l'état retourRapide vers...
    /*
        retourRapide -> play         on versPlay
        retourRapide -> avanceRapide on versAR
        retourRapide -> pause        on versPause
        retourRapide -> fin          on versQuit
    */

    // De l'état avanceRapide vers...
    /*
        avanceRapide -> play         on versPlay
        avanceRapide -> retourRapide on versAR
        avanceRapide -> pause        on versPause
        avanceRapide -> fin          on versQuit
    */

    // De l'état stop vers...
    /*
        stop -> play on versPlay
        stop -> fin  on versQuit
    */

    // Envoie de messages JSON
    void envoieJsonClients(QJsonObject msg);
    void creerJsonEtatCourant(QString etatCourant);

    // Sauvegarde de l'état courant
    QString etatCourant;
    int volumeCourant;

    // Sauvegarde des morceaux de musiques passés
    QList<QString> listeMusiques;


signals:
    // Signaux de changement d'état
    void versPlay();
    void versPause();
    void versRR();
    void versAR();
    void versStop();
    void versQuit();

    // Signaux internes pour les actions à réaliser
    void changeMusique(QString nouvelleMusique);
    void changeVolume(int valeur);


private slots:
    // Slots a executer quand on entre dans un des états
    void quitMPV();
    void setMPVPause();
    void setMPVPlay();
    void setMPVRR();
    void setMPVAR();
    void setMPVStop();

    // Slots a exécuter lors de la reception d'un message sur la socket MPV
    void readMPVSocket();
    void readClientServerSocket();

    // Slots a exécuter lors d'une nouvelle connection ou d'un départ
    void nouveauClient();
    void deconnectionClient();

    // Slots a exécuter pour changer la music courante, le volume, etc...
    void changeMPVMusique(QString nouvelleMusique);
    void changeMPVVolume(int valeur);
    void mute();
    void demute();

};

#endif // SERVEUR_H
