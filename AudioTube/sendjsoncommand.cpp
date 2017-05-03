#include "sendjsoncommand.h"

SendJSONCommand::SendJSONCommand(QObject *parent) :
    QObject(parent),
    mpv(new QLocalSocket(this))
{
    // Connecting to mpv server
    qDebug() << "Connecting to server....";
    mpv->connectToServer("/tmp/mpv-socket");

    // Verify if we are connected
    if(mpv->waitForConnected())
    {
        qDebug() << "Connected to mpv server.";
    }
    else
    {
        mpv->error();
    }

}


SendJSONCommand::~SendJSONCommand()
{
    mpv->disconnectFromServer();
}


/**
* Method : sendRequestToMPV
* Param : QJsonObject, msg - message to send to the mpv server
* Desc : Send a JSON request to the mpv server
* Return : void
**/
void SendJSONCommand::sendRequestToMPV(QJsonObject msg)
{
    // Converting the JSON msg into bytes msg
    QByteArray bytes = QJsonDocument(msg).toJson(QJsonDocument::Compact)+"\n";
    if(mpv!=NULL)
    {
        mpv->write(bytes.data(), bytes.length());
        mpv->flush();
    }
}


/**
* Method : changeMusicOnMPV
* Param : QString, musicName - name of the music to play
* Desc : Create the QJsonObject with the music name and send it to the mpv server
* Return : bool, if success return true, else return false
**/
SendJSONCommand::changeMusicOnMPV(QString musicName)
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("loadfile");
    jsonArr.apend(musicName);

    jsonObject["command"]=jsonArr;

    SendJSONCommand::sendRequestToMPV(jsonObject);
}











