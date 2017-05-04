#include "sendjsoncommandtoserver.h"
#include "codecommandes.h"

SendJSONCommandToServer::SendJSONCommandToServer(QObject *parent, QString servSocket) :
    QObject(parent),
    server(new QLocalSocket(this))
{
    // Connecting to mpv server
    qDebug() << "Connecting to server....";
    server->connectToServer(servSocket);

    // Verify if we are connected
    if(server->waitForConnected())
    {
        qDebug() << "Connected to mpv server.";
    }
    else
    {
        server->error();
    }
}

SendJSONCommandToServer::~SendJSONCommandToServer()
{
    server->disconnectFromServer();
}


void SendJSONCommandToServer::sendRequestToServer(QJsonObject msg){
    // Converting the JSON msg into bytes msg
    QByteArray bytes = QJsonDocument(msg).toJson(QJsonDocument::Compact)+"\n";
    if(server!=NULL)
    {
        server->write(bytes.data(), bytes.length());
        server->flush();
    }
}


void SendJSONCommandToServer::sendMusicToServer(QString musicName)
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    jsonArr.append(changeMusiqueCMD);
    jsonArr.append(musicName);
    jsonObject["command"] = jsonArr;

    SendJSONCommandToServer::sendRequestToServer(jsonObject);
}


void SendJSONCommandToServer::SendPlayToServ()
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    jsonArr.append(playCMD);
    jsonObject["command"] = jsonArr;

    SendJSONCommandToServer::sendRequestToServer(jsonObject);
}

void SendJSONCommandToServer::SendPauseToServ()
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    jsonArr.append(pauseCMD);

    jsonObject["command"] = jsonArr;

    SendJSONCommandToServer::sendRequestToServer(jsonObject);
}

void SendJSONCommandToServer::SendStopToServ()
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    jsonArr.append(stopCMD);

    jsonObject["command"] = jsonArr;

    SendJSONCommandToServer::sendRequestToServer(jsonObject);
}

void SendJSONCommandToServer::SendFastForwardToServ()
{
    // server->setFF()
}

void SendJSONCommandToServer::SendVolumeToServ(float volume)
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    jsonArr.append(changeVolumeCMD);
    jsonArr.append(volume);

    jsonObject["command"] = jsonArr;

    SendJSONCommandToServer::sendRequestToServer(jsonObject);
}

void SendJSONCommandToServer::SendFastReturnToServ()
{
    // server->setFR()
}


void SendJSONCommandToServer::sendQuitToServer()
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    jsonArr.append(quitCMD);

    jsonObject["command"] = jsonArr;

    SendJSONCommandToServer::sendRequestToServer(jsonObject);
}


void SendJSONCommandToServer::sendOffsetToServer(int offset)
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    jsonArr.append(quitCMD);
    jsonArr.append(offset);

    jsonObject["command"] = jsonArr;

    SendJSONCommandToServer::sendRequestToServer(jsonObject);
}
