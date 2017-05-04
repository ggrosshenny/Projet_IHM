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

    JSONToSrv = new SendJSONCommandToServer(0, "/tmp/server-socket");

    qRegisterMetaType<signalType>("signalType");
}

Client::~Client() {
  m_socket->disconnectFromServer();
}

void Client::readSocketOnMPV() {

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


void setVolume(float volume){
    this->JSONToSrv::SendVolumeToServ(volume);
}

void setPlay(){
    this->JSONToSrv::SendPlayToServ();
}

void setPause(){
    this->JSONToSrv::SendPauseToServ();
}

void setStop(){
    this->JSONToSrv::SendStopToServ();
}

void setAR(){
    this->JSONToSrv::SendFastForwardToServ();
}

void setRR(){
    this->JSONToSrv::SendFastReturnToServ();
}

