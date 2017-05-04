#include "sendjsoncommandtoserver.h"

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


SendJSONCommandToServer::sendRequestToServer(QJsonObject msg){
    // Converting the JSON msg into bytes msg
    QByteArray bytes = QJsonDocument(msg).toJson(QJsonDocument::Compact)+"\n";
    if(server!=NULL)
    {
        server->write(bytes.data(), bytes.length());
        server->flush();
    }
}

void SendJSONCommandToServer::SendPlayToServ()
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    jsonArr.append("set_property");
    jsonArr.append("pause");
    jsonArr.append(false);

    jsonObject["command"] = jsonArr;

    SendJSONCommandToServer::sendRequestToServer(jsonObject);
}

void SendJSONCommandToServer::SendPauseToServ()
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    jsonArr.append("set_property");
    jsonArr.append("pause");
    jsonArr.append(true);

    jsonObject["command"] = jsonArr;

    SendJSONCommandToServer::sendRequestToServer(jsonObject);
}

void SendJSONCommandToServer::SendStopToServ()
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    jsonArr.append("pause");

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

    jsonArr.append("set_property");
    jsonArr.append("volume");
    jsonArr.append(volume);

    jsonObject["command"] = jsonArr;

    SendJSONCommandToServer::sendRequestToServer(jsonObject);
}

void SendJSONCommandToServer::SendFastReturnToServ()
{
    // server->setFR()
}
