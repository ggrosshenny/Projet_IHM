#include "sendjsoncommandtoserver.h"

SendJSONCommandToServer::SendJSONCommandToServer(QObject *parent) :
    QObject(parent),
    server(new QLocalSocket(this))
{
    // Connecting to mpv server
    qDebug() << "Connecting to server....";
    server->connectToServer("/tmp/mpv-socket");

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

void SendJSONCommandToServer::setPlay()
{
    // server->setPlay()
}

void SendJSONCommandToServer::setPause()
{
    // server->setPause()
}

void SendJSONCommandToServer::setStop()
{
    // server->setStop()
}

void SendJSONCommandToServer::setFastForward()
{
    // server->setFF()
}

void SendJSONCommandToServer::setFastReturn()
{
    // server->setFR()
}
