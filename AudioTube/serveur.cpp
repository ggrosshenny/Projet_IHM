#include "serveur.h"
#include "sendjsoncommandToMPV.h"

Serveur::Serveur(QObject *parent, QString mpvSocket)
{
    // ---------------------------------------------------
    // Trouver la socket de MPV et créer celle des clients

    // Trouver la socket MPV
    MPVSocket->connectToServer("/tmp/mpv-socket");
    // Creation de l'objet de communication vers MPV
    communicationMPV = new SendjsoncommandToMPV(0, "/tmp/mpv-socket");

    QObject::connect(MPVSocket, SIGNAL(readyRead()), this, SLOT(readMPVSocket()));

    // Verification de la réussite de la connection
    if(MPVSocket->waitForConnected())
    {
        qDebug() << "Connected to mpv server.";
    }
    else
    {
        MPVSocket->error();
    }

    // Création de la socket des clients
    clientServer = new QLocalServer();
    clientServer->setSocketOptions(QLocalServer::UserAccessOption);
    clientServer->listen("/tmp/server-IHM");

    // Connection du serveur avec le slot nouveauClient pour assurer l'enregistrement des nouvelles connections
    QObject::connect(clientServer, SIGNAL(newConnection()), this, SLOT(nouveauClient()));


    // ---------------------------------------------------
    // Definition de la machine à états
    machine =new QStateMachine(this);

    // Definition de ses états

    // Etats de la machine
    play = new QState(machine);
    pause = new QState(machine);
    stop = new QState(machine);
    retourRapide = new QState(machine);
    avanceRapide = new QState(machine);

    // Etat final de la machine
    fin = new QFinalState(machine);

    // Definitions des transitions entre les états
    // De l'état play vers les autres états
    play->addTransition(this, SIGNAL(versPause()), pause);          // play -> pause        on versPause
    play->addTransition(this, SIGNAL(versRR()), retourRapide);      // play -> retourRapide on versRR
    play->addTransition(this, SIGNAL(versAR()), avanceRapide);      // play -> avanceRapide on versAr
    play->addTransition(this, SIGNAL(versStop()), stop);            // play -> stop         on versStop
    play->addTransition(this, SIGNAL(versQuit()), fin);             // play -> fin          on versQuit

    // De l'état pause vers ...
    pause->addTransition(this, SIGNAL(versPlay()), play);   // pause -> play on versPlay
    pause->addTransition(this, SIGNAL(versStop()), stop);   // pause -> stop on versStop
    pause->addTransition(this, SIGNAL(versQuit()), fin);    // pause -> fin  on versQuit

    // De l'état retourRapide vers ...
    retourRapide->addTransition(this, SIGNAL(versPlay()), play);         // RetourRapide -> play         on versPlay
    retourRapide->addTransition(this, SIGNAL(versAR()), avanceRapide);   // RetourRapide -> avanceRapide on versAR
    retourRapide->addTransition(this, SIGNAL(versPause()), pause);       // RetourRapide -> pause        on versPause
    retourRapide->addTransition(this, SIGNAL(versQuit()), fin);          // RetourRapide -> fin          on versQuit

    // De l'état avanceRapide vers ...
    avanceRapide->addTransition(this, SIGNAL(versPlay()), play);        // avanceRapide -> play         on versPlay
    avanceRapide->addTransition(this, SIGNAL(versRR()), retourRapide);  // avanceRapide -> retourRapide on versRR
    avanceRapide->addTransition(this, SIGNAL(versPause()), pause);      // avanceRapide -> pause        on versPause
    avanceRapide->addTransition(this, SIGNAL(versQuit()), fin);         // avanceRapide -> fin          on versQuit

    // De l'état stop vers ...
    stop->addTransition(this, SIGNAL(versPlay()), play);    // stop -> play on versPlay
    stop->addTransition(this, SIGNAL(versQuit()), fin);     // stop -> fin  on versQuit


    // -------------------------------------
    // Connects des états et des slots qui leur sont associés

    // Play
    QObject::connect(play, SIGNAL(entered()), this, SLOT(setMPVPlay()));
    // pause
    QObject::connect(pause, SIGNAL(entered()), this, SLOT(setMPVPause()));
    // RetourRapide
    QObject::connect(retourRapide, SIGNAL(entered()), this, SLOT(setMPVRR()));
    // AvanceRapide
    QObject::connect(avanceRapide, SIGNAL(entered()), this, SLOT(setMPVAR()));
    // stop
    QObject::connect(stop, SIGNAL(entered()),this, SLOT(setMPVStop()));

    // Connects pour les slots de lectures des sockets de MPV et des clients
    connect(MPVSocket, SIGNAL(readyRead()), this, SLOT(readMPVSocket()));

    // Definition de l'état initial
    machine->setInitialState(stop);
    etatCourant = "stop";
    machine->start();
}



/**
 * Method : nouveauClient
 * Param : void
 * Desc : Assure la connection d'un nouveau client sur le serveur
 * Return : void
 **/
void Serveur::nouveauClient()
{
    QLocalSocket *socketNouveauClient = clientServer->nextPendingConnection();

    if(!socketNouveauClient)
    {
        return; // Abandon de la connection
    }

    clientsSocket.append(socketNouveauClient);

    QObject::connect(socketNouveauClient, SIGNAL(readyRead()), this, SLOT(readClientServerSocket()));
    QObject::connect(socketNouveauClient, SIGNAL(disconnected()), this, SLOT(deconnectionClient()));

    creerJsonEtatCourant(etatCourant);

    qDebug() << "Un nouveau client s'est connecté au serveur !";
}


/**
 * Method : deconnectionClient
 * Param : void
 * Desc : Assure la deconnection d'un client au serveur local
 * Return : void
 **/
void Serveur::deconnectionClient()
{
    QLocalSocket *socketClient = qobject_cast<QLocalSocket*>(sender());

    if(!socketClient)
    {
        return;
    }
    clientsSocket.removeOne(socketClient);

    socketClient->deleteLater();

    qDebug() << "Un client s'est déconnecté du serveur !";
}


/**
 * slot : quitMPV
 * Param : void
 * Desc : Appelle la méthode quitMPVServer de l'objet sendJSONCommandTOMPV
 * Return : void
 **/
void Serveur::quitMPV()
{
    communicationMPV->quitMPVServer();
    etatCourant = "quit";
    creerJsonEtatCourant(etatCourant);
}


/**
 * Slot : setMPVPlay
 * Param : void
 * Desc : appelle la methode setMPVPause(false) de l'objet sendjsonCommandTOMPV
 * Return : void
 **/
void Serveur::setMPVPlay()
{
    communicationMPV->setPauseOnMPV(false);
    etatCourant = "play";
    creerJsonEtatCourant(etatCourant);
}


/**
 * Slot : setMPVPause
 * Param : void
 * Desc : appelle la methode setPauseOnMPV(true) de l'objet sendjsonCommandTOMPV
 * Return : void
 **/
void Serveur::setMPVPause()
{
    communicationMPV->setPauseOnMPV(true);
    etatCourant = "pause";
    creerJsonEtatCourant(etatCourant);
}


/**
 * Slot : setMPVRR
 * Param : void
 * Desc :
 * Return : void
 **/
void Serveur::setMPVRR()
{
    // TO-DO
    etatCourant = "RR";
    creerJsonEtatCourant(etatCourant);
}


/**
 * Slot : setMPVAR
 * Param : void
 * Desc :
 * Return : void
 **/
void Serveur::setMPVAR()
{
    // TO-DO
    etatCourant = "RA";
    creerJsonEtatCourant(etatCourant);
}


/**
 * Slot : setMPVStop
 * Param : void
 * Desc : appelle la methode setStopOnMPV de l'objet sendjsonCommandTOMPV
 * Return : void
 **/
void Serveur::setMPVStop()
{
    communicationMPV->setStopOnMPV();
    etatCourant = "stop";
    creerJsonEtatCourant(etatCourant);
}




/**
 * @brief Serveur::readSocket
 **/
void Serveur::readMPVSocket()
{
    while (MPVSocket->canReadLine()) {
        QByteArray line = MPVSocket->readLine().trimmed();

        // Pour le passage en JSON
        QJsonParseError error;
        QJsonDocument jsonDoc=QJsonDocument::fromJson(line, &error);
        QJsonObject jsonObject=jsonDoc.object();
        qDebug() << jsonObject["error"].toString();

        // ICI ON A RECU UN MESSAGE QU'ON PEUT TRAITER

        qDebug() << QString::fromUtf8(line.constData(), line.length());
    }
}


/**
 * Slot : readClientServerSocket
 * Param : void
 * Desc : lit ce qui est sur la socket lorsque le signal readyRead est intercepté
 * Return : void
 **/
void Serveur::readClientServerSocket()
{
    // TO-DO
}



/**
 * Method : envoieJsonClients
 * Param : QJsonObject, msg - le message au format JSON à envoyer
 * Desc : Envoie le message JSON à tous les clients connectés
 * Return : void
 **/
void Serveur::envoieJsonClients(QJsonObject msg)
{
    QLocalSocket *client;
    int i = 0;

    QByteArray bytes = QJsonDocument(msg).toJson(QJsonDocument::Compact)+"\n";

    for(i=0; i<clientsSocket.length(); i++)
    {
        client = clientsSocket[i];
        if(client!=NULL)
        {
            client->write(bytes.data(), bytes.length());
            client->flush();
        }
    }
}


/**
 * Method : creerJsonEtatCourant
 * Param : QString, etatCourant - QString représentant l'état courant
 * Desc : Créer le QJson pour envoyer l'état courant aux clients et l'envoie à tous les clients connectés
 * Return : QJsonObject
 **/
QJsonObject Serveur::creerJsonEtatCourant(QString etatCourant)
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("etat");
    jsonArr.append(etatCourant);

    jsonObject["command"]=jsonArr;

    Serveur::envoieJsonClients(jsonObject);
}









