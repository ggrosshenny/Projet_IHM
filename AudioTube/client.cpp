#include "client.h"

Client::Client(QObject *parent) :
  QObject(parent),
  client_socket(new QLocalSocket(this))
{
    // Connexion au serveur MPV
    client_socket->connectToServer("/tmp/mpv-socket");

    connect(client_socket, SIGNAL(readyRead()), this, SLOT(readSocketOnServer()));

    // Verify if we are connected
    if(client_socket->waitForConnected())
    {
        qDebug() << "Connected to mpv server.";
    }
    else
    {
        client_socket->error();
    }

    // Récupération des données de lecture -> état dans l'automate ; piste ; avancée ; volume
    // Recup données BDD lecteur
  qRegisterMetaType<signalType>("signalType");
  m_running=true;
  m_clientLoopThread=QtConcurrent::run(this, &Client::serverMessageLoop);
}

Client::~Client() {
  m_socket->disconnectFromServer();
  m_running=false;
  m_clientLoopThread.waitForFinished();
}

void Client::readSocketOnServer() {

    while(client_socket->canReadLine()){

        QByteArray readLineFromServer = client_socket->readLine().trimmed();
        QJsonParseError error;
        QJsonDocument jDoc = QJsonDocument::fromJson(readLineFromServer, &error);
        QJsonObject jObj = jDoc.object();

        if(jObj["error"] == "success"){

            switch (jObj["name"]){
                case "volume":
                    // Signal setVolume
                    break;
                case "title":
                    // Signal setTitle
                    break;
                case "duration":
                    // Signal setDuration
                    break;
                case "percent-pos":
                    // Signal setPosition
                    break;
                default:
                    break;
           }
        }
    }
}



